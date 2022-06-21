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
class WeakHandle;

template <typename T>
class StrongHandle {
public:
    StrongHandle(T &ref)
        : handle_(std::make_shared<ObjHandle>(ref)) {}

    T &ref() const {
        return handle_.ref();
    }

private:
    std::shared_ptr<ObjHandle<T>> &handle_;

    friend class WeakHandle<T>;
};

template <typename T>
class WeakHandle {
public:
    WeakHandle() {}

    WeakHandle(StrongHandle<T> &strongHandle)
        : weakPtr_(strongHandle.handle_) {}

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
    void with(CallbackT &&callback) const {
        std::shared_ptr<ObjHandle<T>> objHandle = weakPtr_.lock();
        if (objHandle) {
            callback(objHandle->ref());
        }
    }

private:
    std::weak_ptr<ObjHandle<T>> weakPtr_;
};

}
