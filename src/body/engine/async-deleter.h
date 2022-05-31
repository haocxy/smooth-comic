#pragma once

#include <memory>
#include <functional>

#include "core/thread.h"


namespace myapp {

class AsyncDeleter {
private:

    // 解决 lambda 表达式不支持移动语义的问题，通过拷贝语义实现移动操作
    template <typename T>
    class UniquePtrMover {
    public:
        UniquePtrMover() {}

        UniquePtrMover(std::unique_ptr<T> &&ptr)
            : ptr_(std::move(ptr)) {}

        UniquePtrMover(const UniquePtrMover &other) {
            *this = other;
        }

        UniquePtrMover(UniquePtrMover &&other) {
            *this = other;
        }

        UniquePtrMover &operator=(const UniquePtrMover &other) {
            if (this != &other) {
                ptr_ = std::move(const_cast<UniquePtrMover &>(other).ptr_);
            }
            return *this;
        }

        UniquePtrMover &operator=(UniquePtrMover &&other) {
            *this = other;
            return *this;
        }

        void deleteObj() {
            delete ptr_.release();
        }

    private:
        std::unique_ptr<T> ptr_;
    };

public:
    explicit AsyncDeleter();

    virtual ~AsyncDeleter();

    template <typename T>
    void asyncDelete(std::unique_ptr<T> &&ptr) {
        UniquePtrMover<T> ptrMover(std::move(ptr));
        taskQueue_.push([ptrMover]() mutable {
            ptrMover.deleteObj();
        });
    }

    template <typename T>
    void asyncDelete(T *ptr) {
        asyncDelete(std::unique_ptr<T>(ptr));
    }

private:
    void loop();

private:
    std::jthread thread_;
    BlockQueue<std::function<void()>> taskQueue_;
};


}

