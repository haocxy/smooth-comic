#pragma once

#include <memory>
#include <functional>

#include "util/actor.h"


namespace myapp {

class AsyncDeleter : public actor::ThreadedActor {
public:

    class Deleter {
    public:
        virtual ~Deleter() {}

        virtual void doDelete() = 0;
    };

    template <typename T>
    class DeleterImpl : public Deleter {
    public:
        DeleterImpl(std::unique_ptr<T> &&ptr) : ptr_(std::move(ptr)) {}

        virtual ~DeleterImpl() {}

        virtual void doDelete() override {
            delete ptr_.release();
        }

    private:
        std::unique_ptr<T> ptr_;
    };

    class AsyncDeleteMsg : public actor::Message {
    public:
        template <typename T>
        AsyncDeleteMsg(std::unique_ptr<T> &&ptr)
            : deleter_(std::make_unique<DeleterImpl<T>>(std::move(ptr))) {}

        virtual ~AsyncDeleteMsg() {}

        void doDelete() {
            if (deleter_) {
                deleter_->doDelete();
            }
        }

    private:
        std::unique_ptr<Deleter> deleter_;
    };

public:
    AsyncDeleter();

    virtual ~AsyncDeleter();

    virtual void onMessage(actor::Message &msg);
};


}

