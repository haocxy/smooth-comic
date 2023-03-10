#pragma once

#include <cstring>
#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>
#include <ostream>


namespace ustrdetail
{

namespace utf8detail
{

constexpr char32_t keeplow = 0b0011'1111;
constexpr char32_t decolow = 0b1000'0000;

constexpr char32_t keephi2 = 0b1111'1;
constexpr char32_t keephi3 = 0b1111;
constexpr char32_t keephi4 = 0b111;
constexpr char32_t keephi5 = 0b11;
constexpr char32_t keephi6 = 0b1;

constexpr char32_t decohi2 = 0b1100'0000;
constexpr char32_t decohi3 = 0b1110'0000;
constexpr char32_t decohi4 = 0b1111'0000;
constexpr char32_t decohi5 = 0b1111'1000;
constexpr char32_t decohi6 = 0b1111'1100;



}

inline std::u8string convertToU8(const std::u32string_view &u32s)
{
    using namespace utf8detail;

    std::u8string result;

    for (char32_t u : u32s) {
        if (u < 0x80) {
            result.push_back(static_cast<char8_t>(u));
        } else if (u < 0x800) {
            result.push_back(static_cast<char8_t>(decohi2 | (keephi2 & (u >> 6))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & u)));
        } else if (u < 0x10000) {
            result.push_back(static_cast<char8_t>(decohi3 | (keephi3 & (u >> 12))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 6))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & u)));
        } else if (u < 0x200000) {
            result.push_back(static_cast<char8_t>(decohi4 | (keephi4 & (u >> 18))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 12))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 6))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & u)));
        } else if (u < 0x4000000) {
            result.push_back(static_cast<char8_t>(decohi5 | (keephi5 & (u >> 24))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 18))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 12))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 6))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & u)));
        } else {
            result.push_back(static_cast<char8_t>(decohi6 | (keephi6 & (u >> 30))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 24))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 18))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 12))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & (u >> 6))));
            result.push_back(static_cast<char8_t>(decolow | (keeplow & u)));
        }
    }

    return result;
}

inline std::u32string convertToU32(const std::u8string &u8s)
{
    using namespace utf8detail;

    std::u32string result;
    result.reserve(u8s.size());

    const auto nbytes = u8s.size();

    std::remove_const_t<decltype(nbytes)> i = 0;

    while (i < nbytes) {
        const char32_t c1 = u8s[i++];
        if ((c1 & 0b1000'0000) == 0) {
            result.push_back(c1);
            continue;
        }
        const char32_t c2 = u8s[i++];
        if ((c1 & 0b1110'0000) == 0b1100'0000) {
            char32_t u1 = (c1 & 0b0001'1111);
            char32_t u2 = (c2 & 0b0011'1111);
            char32_t u = (u1 << 6) | u2;
            result.push_back(u);
            continue;
        }
        const char32_t c3 = u8s[i++];
        if ((c1 & 0b1111'0000) == 0b1110'0000) {
            char32_t u1 = (c1 & 0b0000'1111);
            char32_t u2 = (c2 & 0b0011'1111);
            char32_t u3 = (c3 & 0b0011'1111);
            char32_t u = (u1 << 12) | (u2 << 6) | u3;
            result.push_back(u);
            continue;
        }
        const char32_t c4 = u8s[i++];
        if ((c1 & 0b1111'1000) == 0b1111'0000) {
            char32_t u1 = (c1 & 0b0000'0111);
            char32_t u2 = (c2 & 0b0011'1111);
            char32_t u3 = (c3 & 0b0011'1111);
            char32_t u4 = (c4 & 0b0011'1111);
            char32_t u = (u1 << 18) | (u2 << 12) | (u3 << 6) | u4;
            result.push_back(u);
            continue;
        }
        const char32_t c5 = u8s[i++];
        if ((c1 & 0b1111'1100) == 0b1111'1000) {
            char32_t u1 = (c1 & 0b0000'0011);
            char32_t u2 = (c2 & 0b0011'1111);
            char32_t u3 = (c3 & 0b0011'1111);
            char32_t u4 = (c4 & 0b0011'1111);
            char32_t u5 = (c5 & 0b0011'1111);
            char32_t u = (u1 << 24) | (u2 << 18) | (u3 << 12) | (u4 << 6) | u5;
            result.push_back(u);
            continue;
        }
        const char32_t c6 = u8s[i++];
        if ((c1 & 0b1111'1110) == 0b1111'1100) {
            char32_t u1 = (c1 & 0b0000'0001);
            char32_t u2 = (c2 & 0b0011'1111);
            char32_t u3 = (c3 & 0b0011'1111);
            char32_t u4 = (c4 & 0b0011'1111);
            char32_t u5 = (c5 & 0b0011'1111);
            char32_t u6 = (c6 & 0b0011'1111);
            char32_t u = (u1 << 30) | (u2 << 24) | (u3 << 18) | (u4 << 12) | (u5 << 6) | u6;
            result.push_back(u);
            continue;
        }
    }

    return result;
}

}


class u8str : public std::u8string {
public:
    u8str() {}

    explicit u8str(const std::u8string &s)
        : std::u8string(s) {}

    explicit u8str(std::u8string &&s)
        : std::u8string(std::move(s)) {}

    explicit u8str(const std::string &s)
        : std::u8string(s.begin(), s.end()) {}

    u8str(const std::u8string_view &view)
        : std::u8string(view) {}

    u8str(const u8str &b)
        : std::u8string(b) {}

    u8str(u8str &&b) noexcept
        : std::u8string(std::move(b)) {}

    u8str(const std::u32string &u32str)
        : std::u8string(ustrdetail::convertToU8(u32str)) {}

    u8str(const std::u32string_view &u32str)
        : std::u8string(ustrdetail::convertToU8(u32str)) {}

    u8str(allocator_type allocator)
        : std::u8string(allocator) {}

    u8str(const char *cstr)
        : u8str(std::string(cstr)) {}

    u8str(short n)
        : u8str(std::to_string(n)) {}

    u8str(int n)
        : u8str(std::to_string(n)) {}

    u8str(long n)
        : u8str(std::to_string(n)) {}

    u8str(long long n)
        : u8str(std::to_string(n)) {}

    operator std::string() const {
        return std::string(begin(), end());
    }

    virtual ~u8str() {}

    u8str &operator=(const u8str &b) {
        std::u8string::operator=(b);
        return *this;
    }

    u8str &operator=(u8str &&b) noexcept {
        std::u8string::operator=(std::move(b));
        return *this;
    }

    u8str operator+(const u8str &b) const {
        return u8str(
            static_cast<const std::u8string &>(*this)
            + static_cast<const std::u8string &>(b)
        );
    }

    operator std::u32string() const {
        return ustrdetail::convertToU32(*this);
    }

    operator int() const {
        return std::stoi(*this);
    }

    operator long() const {
        return std::stol(*this);
    }

    operator long long() const {
        return std::stoll(*this);
    }
};

class u8view : public std::u8string_view {
public:

    u8view() {}

    u8view(const u8str &s)
        : std::u8string_view(s) {}

    template <typename... Args>
    u8view(Args &&...args)
        : std::u8string_view(std::forward<Args>(args)...) {}

    virtual ~u8view() {}
};

using u16str = std::u16string;

using u16view = std::u16string_view;

class u32str : public std::u32string {
public:

    u32str() {}

    u32str(const std::u32string &b)
        : std::u32string(b) {}

    u32str(std::u32string &&b)
        : std::u32string(std::move(b)) {}

    u32str(const u32str &b)
        : std::u32string(b) {}

    u32str(u32str &&b) noexcept
        : std::u32string(std::move(b)) {}

    u32str(allocator_type allocator)
        : std::u32string(allocator) {}

    u32str(const u8str &u8s)
        : std::u32string(ustrdetail::convertToU32(u8s)) {}

    virtual ~u32str() {}

    u32str &operator=(const std::u32string &b) {
        std::u32string::operator=(b);
        return *this;
    }

    u32str &operator=(std::u32string &&b) {
        std::u32string::operator=(std::move(b));
        return *this;
    }

    u32str &operator=(const u32str &b) {
        std::u32string::operator=(b);
        return *this;
    }

    u32str &operator=(u32str &&b) noexcept {
        std::u32string::operator=(std::move(b));
        return *this;
    }
};

using u32view = std::u32string_view;
