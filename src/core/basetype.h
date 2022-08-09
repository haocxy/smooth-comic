#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <optional>

#include "scoped_container.h"
#include "ustr.h"


using i8 = int8_t;
using u8 = uint8_t;

using i16 = int16_t;
using u16 = uint16_t;

using i32 = int32_t;
using u32 = uint32_t;
using f32 = float;
static_assert(sizeof(float) == 4);

using i64 = int64_t;
using u64 = uint64_t;
using f64 = double;
static_assert(sizeof(double) == 8);

using cstr = const char *;

using str = std::string;

template <typename T>
class uptr : public std::unique_ptr<T> {
public:
    uptr() {}

    template <typename U>
    uptr(const uptr<U> &other) = delete;

    template <typename U>
    uptr(uptr<U> &&other) noexcept
        : std::unique_ptr<T>(std::move(other)) {}

    uptr(nullptr_t) {}

    template <typename U>
    uptr(U *&&p) : std::unique_ptr<T>(p) {
        p = nullptr;
    }

    template <typename U>
    uptr(std::unique_ptr<U> &&other)
        : std::unique_ptr<T>(std::move(other)) {}

    virtual ~uptr() {}

    template <typename U>
    uptr &operator=(const uptr<U> &other) = delete;

    template <typename U>
    uptr &operator=(uptr<U> &&other) noexcept {
        std::unique_ptr<T>::operator=(std::move(other));
        return *this;
    }

    template <typename U>
    uptr &operator=(std::unique_ptr<U> &&other) noexcept {
        std::unique_ptr<T>::operator=(std::move(other));
        return *this;
    }

    uptr &operator=(nullptr_t p) {
        std::unique_ptr<T>::operator=(p);
        return *this;
    }
};

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using opt = std::optional<T>;

using Buff = std::basic_string<std::byte>;
