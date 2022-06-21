#pragma once

#include <memory>


namespace myapp {

template <typename T>
class ObjHandle : public std::enable_shared_from_this<ObjHandle<T>> {
public:
    ObjHandle(T &ref) : ref_(ref) {}

    T &ref() const {
        return ref_;
    }

private:
    T &ref_;
};

template <typename T>
class StrongHandle;

template <typename T>
class WeakHandle {
public:
    WeakHandle() {}

    WeakHandle(const WeakHandle &other)
        : weakPtr_(other.weakPtr_) {}

    WeakHandle(WeakHandle &&other) noexcept
        : weakPtr_(std::move(other.weakPtr_)) {}

    WeakHandle &operator=(const WeakHandle &other) {
        weakPtr_ = other.weakPtr_;
        return *this;
    }

    WeakHandle &operator=(WeakHandle &&other) {
        weakPtr_ = std::move(weakPtr_);
        return *this;
    }

    std::shared_ptr<ObjHandle<T>> obj() const {
        return weakPtr_.lock();
    }

    template <typename CallbackT>
    void apply(CallbackT &&callback) const {
        std::shared_ptr<ObjHandle<T>> objHandle = weakPtr_.lock();
        if (objHandle) {
            callback(objHandle->ref());
        }
    }

private:
    WeakHandle(std::weak_ptr<ObjHandle<T>> weakPtr)
        : weakPtr_(weakPtr) {}

private:
    std::weak_ptr<ObjHandle<T>> weakPtr_;

    friend class StrongHandle<T>;
};

template <typename T>
class StrongHandle {
public:
    StrongHandle(T &ref)
        : handle_(std::make_shared<ObjHandle<T>>(ref)) {}

    T &ref() const {
        return handle_.ref();
    }

    WeakHandle<T> weak() const {
        return WeakHandle<T>(handle_);
    }

private:
    std::shared_ptr<ObjHandle<T>> handle_;
};



}
