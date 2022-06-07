// 提供只需要前置声明即可工作的智能指针
// 定义指针时，只需要提供对应类的前置声明，知道要使其指向某个具体对象时才需要包含定义

#pragma once

#include <utility>
#include <memory>


namespace myapp {


namespace detail {

class DeclarePtrHolder {
public:
    virtual ~DeclarePtrHolder() {}

    virtual void *ptr() const = 0;
};

template <typename T>
class DeclarePtrHolderImpl : public DeclarePtrHolder {
public:
    DeclarePtrHolderImpl() {}

    DeclarePtrHolderImpl(T *ptr) : ptr_(ptr) {}

    virtual ~DeclarePtrHolderImpl() {
        delete ptr_;
        ptr_ = nullptr;
    }

    virtual void *ptr() const override {
        return ptr_;
    }

private:
    T *ptr_{};
};

template <typename T, typename RefT>
class BasicDeclarePtr {
public:
    using pointer = std::remove_reference_t<RefT> *;
    using element_type = T;
    static_assert(std::is_same_v<std::add_pointer_t<element_type>, pointer>);

    BasicDeclarePtr() {}

    BasicDeclarePtr(T *&&ptr)
        : holder_(std::make_unique<DeclarePtrHolderImpl<T>>(ptr)) {

        ptr = nullptr;
    }

    template <typename T2, typename RefT2>
    BasicDeclarePtr(BasicDeclarePtr<T2, RefT2> &&other)
        : holder_(std::move(other.holder_)) {}

    template <typename T2, typename RefT2>
    BasicDeclarePtr &operator=(BasicDeclarePtr<T2, RefT2> &&other) {
        holder_ = std::move(other.holder_);
        return *this;
    }

    // 创建一个新的指针，指向新的对象
    template <typename... Args>
    static BasicDeclarePtr create(Args&& ...args) {
        return BasicScopedPtr(new T(std::forward<Args>(args)...));
    }

    // 使这个指针指向一个新创建的对象，若原来有指向的对象则先释放原来指向的对象
    template <typename... Args>
    void emplace(Args&& ...args) {
        holder_ = std::make_unique<DeclarePtrHolderImpl<T>>(new T(std::forward<Args>(args)...));
    }

    operator bool() const {
        return holder_ && (holder_->ptr());
    }

    pointer operator->() const {
        return reinterpret_cast<pointer>(holder_->ptr());
    }

    RefT operator*() const {
        return *reinterpret_cast<pointer>(holder_->ptr());
    }

private:
    std::unique_ptr<DeclarePtrHolder> holder_;
};

} // namespace detail


// 指向可变对象的声明指针
// “声明指针”的含义：在定义对象时只需要声明指向类型，只要在创建所指向的对象时包含对应类的定义即可
template <typename T>
using DeclarePtr = detail::BasicDeclarePtr<T, T &>;


// 指向不可变对象的声明指针
// “声明指针”的含义：在定义对象时只需要声明指向类型，只要在创建所指向的对象时包含对应类的定义即可
template <typename T>
using DeclareConstPtr = detail::BasicDeclarePtr<T, const T &>;

} // namespace myapp
