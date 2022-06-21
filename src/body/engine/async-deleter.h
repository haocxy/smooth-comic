#pragma once

#include <memory>
#include <functional>

#include "core/thread.h"


namespace myapp {

class AsyncDeleter {
public:
    AsyncDeleter();

    ~AsyncDeleter();

    template <typename T>
    void asyncDelete(std::unique_ptr<T> &&ptr) {
        worker_.exec([p = std::move(ptr)]{
            delete p.release();
        });
    }

private:
    SingleThreadStrand worker_;
};

}
