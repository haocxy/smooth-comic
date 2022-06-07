#pragma once

#include <map>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>


namespace myapp::strutil
{

template <typename T>
std::string convert(const T &obj) {
    std::ostringstream ss;
    ss << obj;
    return ss.str();
}

template <typename S>
class StrImplicitConverter
{
public:
    StrImplicitConverter(const S &s) : s_(s){}

    template <typename T>
    operator T() const {
        std::string str(s_);
        std::istringstream ss(str);
        T result{};
        ss >> result;
        return result;
    }

private:
    const S &s_;
};

inline StrImplicitConverter<std::string> convert(const std::string &s) {
    return StrImplicitConverter(s);
}

inline StrImplicitConverter<std::string_view> convert(const std::string_view &s) {
    return StrImplicitConverter(s);
}

template <typename T>
inline std::string tostr(const T &obj) {
    std::ostringstream ss;
    ss << obj;
    return ss.str();
}

bool isdigit(char c);

bool isblank(char c);

bool isblank(const std::string_view &s);

class strviews
{
public:
    using value_type = std::string_view;
    using size_type = std::vector<value_type>::size_type;
    using const_iterator = std::vector<value_type>::const_iterator;
    using iterator = std::vector<value_type>::iterator;

    strviews() {}

    strviews(const strviews &b) : views_(b.views_) {}

    strviews(strviews &&b) noexcept : views_(std::move(b.views_)) {}

    strviews &operator=(const strviews &b) {
        views_ = b.views_;
        return *this;
    }

    strviews &operator=(strviews &&b) noexcept {
        views_ = std::move(b.views_);
        return *this;
    }

    bool empty() const {
        return views_.empty();
    }

    size_type size() const {
        return views_.size();
    }

    const value_type &operator[](size_type i) const {
        if (i < views_.size() && i >= 0) {
            return views_[i];
        } else {
            static const std::string_view s_empty;
            return s_empty;
        }
    }

    void add(const std::string_view &view) {
        views_.push_back(view);
    }

    const_iterator begin() const {
        return views_.begin();
    }

    const_iterator end() const {
        return views_.end();
    }

    iterator begin() {
        return views_.begin();
    }

    iterator end() {
        return views_.end();
    }

private:
    std::vector<value_type> views_;
};

strviews split(const std::string_view &str, char sep);

strviews split(const std::string_view &str, char sep, size_t limit);

std::string_view trim(const std::string_view &str);

bool equalIgnoreCase(const std::string_view &a, const std::string_view &b);

void removeLineComment(std::string &line, char sep);

void removeLineEnd(std::string &line);

bool startWith(const std::string_view &str, const std::string_view &prefix);

bool endWith(const std::string_view &str, const std::string_view &suffix);

std::string_view ext(const std::string_view &str);

std::string_view firstWord(const std::string_view &str);

std::string interpolate(const std::string_view &str, const std::map<std::string, std::string> &kv, const char *defaultValue = "");

inline std::string interpolate(const std::string_view &str, const std::string_view &key, const std::string &val) {
    std::map<std::string, std::string> kv;
    kv[std::string(key)] = std::string(val);
    return interpolate(str, kv);
}

} // namespace strutil
