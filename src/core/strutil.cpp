#include "strutil.h"


namespace myapp::strutil
{

static inline bool isAsciiChar(char c) {
    return static_cast<signed char>(c) > 0;
}

bool isdigit(char c) {
    if (isAsciiChar(c)) {
        return std::isdigit(c);
    } else {
        return false;
    }
}

bool isblank(char c) {
    if (isAsciiChar(c)) {
        return std::isblank(c);
    } else {
        return false;
    }
}

bool isblank(const std::string_view &s) {
    for (char c : s) {
        if (!isblank(c)) {
            return false;
        }
    }
    return true;
}

strviews split(const std::string_view &str, char sep) {
    using Size = std::string_view::size_type;
    strviews views;
    const Size len = str.length();
    Size begin = 0;
    for (Size i = 0; i < len; ++i) {
        if (str[i] == sep) {
            views.add(str.substr(begin, i - begin));
            begin = i + 1;
        }
    }
    if (begin < len) {
        views.add(str.substr(begin));
    }
    return views;
}

strviews split(const std::string_view &str, char sep, size_t limit) {
    using Size = std::string_view::size_type;
    strviews views;
    if (limit == 0 || limit == 1) {
        views.add(str);
        return views;
    }
    const Size len = str.length();
    Size begin = 0;
    for (Size i = 0; i < len; ++i) {
        if (str[i] == sep) {
            if (views.size() + 1 < limit) {
                views.add(str.substr(begin, i - begin));
                begin = i + 1;
            } else {
                views.add(str.substr(begin));
                return views;
            }
        }
    }
    if (begin < len) {
        views.add(str.substr(begin));
    }
    return views;
}

std::string_view trim(const std::string_view &str) {
    const auto len = str.length();
    using Index = std::string_view::size_type;

    Index left = 0;
    for (; left < len; ++left) {
        if (!isblank(str[left])) {
            break;
        }
    }

    Index right = len - 1;
    for (; right >= left && right < len; --right) {
        if (!isblank(str[right])) {
            break;
        }
    }

    if (left <= right) {
        return str.substr(left, right - left + 1);
    } else {
        return std::string_view();
    }
}

bool equalIgnoreCase(const std::string_view &a, const std::string_view &b) {
    using Size = decltype(a.length());
    const Size len = a.length();
    if (len != b.length()) {
        return false;
    }
    for (Size i = 0; i < len; ++i) {
        if (std::tolower(a[i]) != std::tolower(b[i])) {
            return false;
        }
    }
    return true;
}

void removeLineComment(std::string &line, char sep) {
    using Size = decltype(line.size());
    const Size len = line.size();
    for (Size i = 0; i < len; ++i) {
        if (line[i] == sep) {
            line.resize(i);
            return;
        }
    }
}

void removeLineEnd(std::string &line) {
    const auto len = line.length();
    for (auto i = len - 1; i >= 0 && i < len; --i) {
        const char c = line[i];
        if (c == '\n' || c == '\r') {
            line.pop_back();
        } else {
            return;
        }
    }
}

bool startWith(const std::string_view &str, const std::string_view &prefix) {
    return str.find(prefix) == 0;
}

bool endWith(const std::string_view &str, const std::string_view &suffix) {
    const auto strLen = str.length();
    const auto suffixLen = suffix.length();
    if (suffixLen > strLen) {
        return false;
    }

    auto suffixIndex = suffixLen - 1;
    auto strIndex = strLen - 1;

    for (; suffixIndex >= 0 && suffixIndex < suffixLen; --suffixIndex, --strIndex) {

        if (str[strIndex] != suffix[suffixIndex]) {
            return false;
        }
    }

    return true;
}

std::string_view ext(const std::string_view &str) {
    const auto len = str.length();
    for (auto i = len - 1; i < len && i >= 0; --i) {
        if (str[i] == '.') {
            return str.substr(i + 1);
        }
    }
    return std::string_view();
}

std::string_view firstWord(const std::string_view &str) {
    using Size = std::string_view::size_type;

    const Size len = str.length();

    bool leftFound = false;
    Size left = 0;

    for (Size i = 0; i < len; ++i) {
        if (isblank(str[i])) {
            if (leftFound) {
                return str.substr(left, i - left);
            } else {
                continue;
            }
        } else {
            if (leftFound) {
                continue;
            } else {
                left = i;
                leftFound = true;
            }
        }
    }

    if (leftFound) {
        return str.substr(left);
    } else {
        return std::string_view();
    }
}

std::string interpolate(const std::map<std::string, std::string> &kv, const std::string &str, const char *defaultValue) {
    std::string result;
    std::string argName;

    enum class State
    {
        Normal, NormalEscape, DollarFound, Arg, ArgEscape
    };

    State st = State::Normal;

    for (char c : str) {
        switch (st) {
        case State::Normal:
            if (c == '\\') {
                st = State::NormalEscape;
            } else if (c == '$') {
                st = State::DollarFound;
            } else {
                result.push_back(c);
            }
            break;
        case State::NormalEscape:
            result.push_back(c);
            st = State::Normal;
            break;
        case State::DollarFound:
            if (c == '{') {
                st = State::Arg;
            } else {
                result.push_back('$');
                result.push_back(c);
                st = State::Normal;
            }
            break;
        case State::Arg:
            if (c == '\\') {
                st = State::ArgEscape;
            } else if (c == '}') {
                auto it = kv.find(argName);
                if (it != kv.end()) {
                    result.append(it->second);
                } else {
                    if (defaultValue) {
                        result.append(defaultValue);
                    }
                }
                argName.clear();
                st = State::Normal;
            } else {
                argName.push_back(c);
            }
            break;
        case State::ArgEscape:
            argName.push_back(c);
            st = State::Arg;
            break;
        default:
            break;
        }
    }

    return result;
}

} // namespace strutil
