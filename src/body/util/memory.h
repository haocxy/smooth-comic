#pragma once

#include <cassert>

#include <limits>
#include <mutex>
#include <condition_variable>
#include <memory_resource>


namespace myapp {

class LimitedMemPool : public std::pmr::memory_resource {
public:
    LimitedMemPool(size_t limit = std::numeric_limits<size_t>::max())
        : limit_(limit) {}

    virtual ~LimitedMemPool() {}

    virtual void *do_allocate(std::size_t bytes, std::size_t alignment) override {
        Lock lock(mtx_);
        size_t newUsage = usage_ + bytes;
        while (!(newUsage <= limit_ && newUsage >= usage_)) { // 和当前使用量比较是为了防回绕
            cvCanAllocate_.wait(lock);
        }
        void *ret = resource_.allocate(bytes, alignment);
        usage_ = newUsage;
        return ret;
    }

    virtual void do_deallocate(void *p, std::size_t bytes, std::size_t alignment) override {
        Lock lock(mtx_);
        assert(usage_ >= bytes);
        resource_.deallocate(p, bytes, alignment);
        usage_ -= bytes;
        cvCanAllocate_.notify_all();
    }

    virtual bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override {
        return this == &other;
    }

private:
    using Mtx = std::mutex;
    using Lock = std::unique_lock<Mtx>;
    mutable Mtx mtx_;
    mutable std::condition_variable cvCanAllocate_;
    const size_t limit_;
    size_t usage_{};
    std::pmr::unsynchronized_pool_resource resource_;
};

}
