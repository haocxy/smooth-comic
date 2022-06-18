#pragma once

#include <memory>


namespace myapp {

template <typename T>
class ObjHandle : public std::enable_shared_from_this<ObjHandle<T>> {
public:
    ObjHandle(T &ref) : ref_(ref) {}

    const T &ref() const {
        return ref_;
    }

    T &ref() {
        return ref_;
    }

private:
    T &ref_;
};

}
