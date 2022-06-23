#pragma once

#include <set>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <string>
#include <limits>
#include <optional>
#include <functional>
#include <condition_variable>

#include "obj-handle.h"


namespace ThreadUtil
{

uint64_t currentThreadShortId();

void setNameForCurrentThread(const std::string_view &name);

const std::string &currentThreadName();

}

template <typename T>
class FifoQueue {
public:
    using value_type = T;

    FifoQueue(size_t limit = std::numeric_limits<size_t>::max())
        : limit_(limit) {}

    size_t size() const {
        return queue_.size();
    }

    bool canPush() const {
        return queue_.size() < limit_;
    }

    void push(T &&obj) {
        queue_.push(std::forward<T>(obj));
    }

    bool canPop() const {
        return !queue_.empty();
    }

    T pop() {
        T first(std::move(queue_.front()));
        queue_.pop();
        return first;
    }

private:
    std::queue<T> queue_;
    size_t limit_ = std::numeric_limits<size_t>::max();
};

template <typename T, typename PriorityT, typename CmpT = std::greater<PriorityT>>
class PriorityQueue {
public:
    using value_type = T;

    PriorityQueue(size_t limit = std::numeric_limits<size_t>::max())
        : limit_(limit) {}

    size_t size() const {
        return size_;
    }

    bool canPush() const {
        return size_ < limit_;
    }

    void push(T &&obj, PriorityT priority) {
        queues_[priority].push(std::forward<T>(obj));
        ++size_;
    }

    bool canPop() const {
        return size_ > 0;
    }

    T pop() {
        auto end = queues_.end();
        for (auto it = queues_.begin(); it != queues_.end(); ++it) {
            auto &queue = it->second;
            if (!(queue.empty())) {
                T result(std::move(queue.front()));
                queue.pop();
                --size_;
                if (queue.empty()) {
                    queues_.erase(it);
                }
                return result;
            }
        }
        throw std::logic_error("PriorityQueue::pop() bad logic: must use canPop() before pop()");
    }

private:
    std::map<PriorityT, std::queue<T>, CmpT> queues_;
    size_t limit_ = std::numeric_limits<size_t>::max();
    size_t size_{ 0 };
};

template <typename QueueType>
class BasicBlockQueue {
private:
    using value_type = typename QueueType::value_type;
    using Lock = std::unique_lock<std::mutex>;

public:

    BasicBlockQueue() {}

    BasicBlockQueue(size_t limit)
        : queue_(limit) {}

    int32_t size() const {
        Lock lock(mtx_);
        return static_cast<int32_t>(queue_.size());
    }

    void stop() {
        Lock lock(mtx_);
        stopping_ = true;
        cvhasCapa_.notify_all();
        cvhasElem_.notify_all();
    }

    template <typename ...ExtArgs>
    bool push(value_type &&e, ExtArgs &&...extArgs) {
        Lock lock(mtx_);
        if (stopping_) {
            return false;
        }
        while (!queue_.canPush()) {
            cvhasCapa_.wait(lock);
            if (stopping_) {
                return false;
            }
        }
        queue_.push(std::forward<value_type>(e), std::forward<ExtArgs>(extArgs)...);
        cvhasElem_.notify_one();
        return true;
    }

    std::optional<value_type> pop() {
        Lock lock(mtx_);
        if (stopping_) {
            return std::nullopt;
        }
        while (!queue_.canPop()) {
            cvhasElem_.wait(lock);
            if (stopping_) {
                return std::nullopt;
            }
        }
        value_type e(queue_.pop());
        cvhasCapa_.notify_one();
        return e;
    }

private:
    mutable std::mutex mtx_;
    mutable std::condition_variable cvhasElem_;
    mutable std::condition_variable cvhasCapa_;
    bool stopping_ = false;
    QueueType queue_;
};

template <typename T>
using BlockQueue = BasicBlockQueue<FifoQueue<T>>;

template <typename T, typename PriorityType>
using PriorityBlockQueue = BasicBlockQueue<PriorityQueue<T, PriorityType>>;

template <typename F>
using TaskQueue = BlockQueue<std::function<F>>;


// 抽象的可以执行某个操作的实体（只执行，不保证被执行操作的时序）
class Executor {
public:
    using Task = std::function<void()>;

    Executor() {}

    virtual ~Executor() {}

    virtual void post(Task &&task) = 0;
};

// 确保被投递的操作可以按照其投递顺序执行（只保证时许，和在哪个线程执行无关）
// 目的是为了复用线程
class StrandEntry : public Executor {
public:
    using Task = Executor::Task;

    virtual ~StrandEntry() {}

    virtual void exec(Task &&task) {
        if (inThread()) {
            task();
        } else {
            post(std::move(task));
        }
    }

    virtual void post(Task &&task) = 0;

protected:
    virtual bool inThread() const = 0;
};


class StrandAllocator {
public:
    virtual ~StrandAllocator() {}

    virtual StrandEntry *allocate() = 0;
};


class Strand : public Executor {
public:
    using Task = Executor::Task;

    Strand(StrandAllocator &strandAllocator)
        : entry_(strandAllocator.allocate()) {}

    void stop() {
        entry_ = nullptr;
    }

    void exec(Task &&task) {
        entry_->exec(std::move(task));
    }

    void post(Task &&task) {
        entry_->post(std::move(task));
    }

private:
    std::unique_ptr<StrandEntry> entry_;
};


class StrandPool : public StrandAllocator {
private:

    class TaskPack {
    public:
        TaskPack(void *delegatePointer, StrandEntry::Task &&task)
            : delegatePointer_(delegatePointer), task_(std::move(task)) {}

        void *delegatePointer() {
            return delegatePointer_;
        }

        StrandEntry::Task &task() {
            return task_;
        }

    private:
        void *delegatePointer_;
        StrandEntry::Task task_;
    };

    class Worker {
    public:
        Worker(const std::string &name, StrandPool &strandPool);

        ~Worker();

        int32_t queueSize() const {
            return taskQueue_.size();
        }

        bool inThread() const {
            return std::this_thread::get_id() == thread_.get_id();
        }

        void post(void *delegatePointer, StrandEntry::Task &&task) {
            taskQueue_.push(TaskPack(delegatePointer, std::move(task)));
        }

    private:
        void threadBody();

    private:
        StrandPool &strandPool_;
        std::atomic_bool &stopping_;
        
        BlockQueue<TaskPack> taskQueue_;
        std::thread thread_;
    };

public:
    StrandPool(const std::string &name, int32_t threadCount);

    virtual ~StrandPool();

    virtual StrandEntry *allocate();

    class DelegateStrand : public StrandEntry {
    public:
        DelegateStrand(StrandPool &strandPool, Worker &worker);

        virtual ~DelegateStrand();

        virtual void post(StrandEntry::Task &&task) {
            worker_.post(this, std::move(task));
        }

    protected:
        virtual bool inThread() const {
            return worker_.inThread();
        }

    private:
        StrandPool &strandPool_;
        Worker &worker_;
    };

private:
    void initWorkers(const std::string &name, int32_t threadCount);

    Worker &chooseWorker();

    bool isStrandLiving(DelegateStrand *delegateStrand);

    void insertLivingDelegate(DelegateStrand *delegateStrand);

    void removeLivingDelegate(DelegateStrand *delegateStrand);

private:
    using MtxForLivingDelegates = std::shared_mutex;
    using WriteLockForLivingDelegates = std::lock_guard<MtxForLivingDelegates>;
    using ReadLockForLivingDelegates = std::shared_lock<MtxForLivingDelegates>;
    mutable MtxForLivingDelegates mtxForLivingDelegates_;
    std::set<DelegateStrand *> livingDelegates_;

    std::atomic_bool stopping_{ false };

    using MtxForWorkers = std::mutex;
    using LockForWorkers = std::lock_guard<MtxForWorkers>;
    mutable MtxForWorkers mtxForWorkers_;

    std::vector<std::unique_ptr<Worker>> workers_;
};

template <typename QueueType>
class BasicSingleThreadStrand {
public:
    using Task = std::function<void()>;

    BasicSingleThreadStrand(const std::string &name) {
        thread_ = std::thread([this, name] {
            ThreadUtil::setNameForCurrentThread(name);
            threadBody();
        });
    }

    virtual ~BasicSingleThreadStrand() {
        stopEventQueue();
        thread_.join();
    }

    void stopEventQueue() {
        CallTaskLock lock(mtxCallTask_);
        stopping_ = true;
        queue_.stop();
    }

    template <typename ...ExtArgs>
    void exec(Task &&task, ExtArgs &&...extArgs) {
        if (inThread()) {
            task();
        } else {
            if (!stopping_) {
                queue_.push(std::move(task), std::forward<ExtArgs>(extArgs)...);
            }
        }
    }

private:
    bool inThread() const {
        return std::this_thread::get_id() == thread_.get_id();
    }

    void threadBody() {
        while (!stopping_) {
            std::optional<Task> &&task = queue_.pop();

            {
                CallTaskLock lock(mtxCallTask_);
                if (!stopping_ && task && *task) {
                    (*task)();
                }
            }
        }
    }

private:
    std::atomic_bool stopping_{ false };
    std::thread thread_;
    QueueType queue_;
    using CallTaskMtx = std::recursive_mutex;
    using CallTaskLock = std::lock_guard<CallTaskMtx>;
    CallTaskMtx mtxCallTask_;
};

using SingleThreadStrand = BasicSingleThreadStrand<
    BasicBlockQueue<
        FifoQueue<std::function<void()>>
    >
>;

template <typename PriorityType>
using PrioritySingleThreadStrand = BasicSingleThreadStrand<
    BasicBlockQueue<
        PriorityQueue<std::function<void()>, PriorityType>
    >
>;
