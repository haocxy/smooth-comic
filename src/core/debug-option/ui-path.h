#pragma once

#include <stdexcept>
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "core/fs.h"
#include "option-convert.h"


namespace debug_option {

// 在和用户交互的场合使用，例如命令行或者配置
// 提供一致的路径逻辑，例如把 ~ 解析为家目录
class UiPath
{
public:
    class HomePathEnvironmentNotDefined : public std::runtime_error
    {
    public:
        HomePathEnvironmentNotDefined(const std::string_view &name)
            : std::runtime_error(makeMsg(name)) {
        }

        virtual ~HomePathEnvironmentNotDefined() {}

    private:
        static std::string makeMsg(const std::string_view &name) {
            std::string msg;
            msg.append("Home path environment not found: [");
            msg.append(name);
            msg.append("]");
            return msg;
        }
    };

public:
    UiPath() {}

    UiPath(const UiPath &path)
        : p_(path.p_) {
    }

    UiPath(UiPath &&b) noexcept
        : p_(std::move(b.p_)) {
    }

    UiPath(const std::string_view &str)
        : p_(parseString(str)) {
    }

    UiPath &operator=(const UiPath &other) {
        p_ = other.p_;
        return *this;
    }

    UiPath &operator=(UiPath &&b) noexcept {
        p_ = std::move(b.p_);
        return *this;
    }

    UiPath &operator=(const std::string_view &str) {
        p_ = parseString(str);
        return *this;
    }

    UiPath &operator=(const OptionRawData &pref) {
        p_ = parsePreference(pref);
        return *this;
    }

    operator fs::path() const {
        return fs::absolute(p_);
    }

    const std::string &str() const {
        return p_;
    }

    operator const std::string &() const {
        return p_;
    }

    operator std::string_view() const {
        return p_;
    }

    void print(std::ostream &os) const {
        os << p_;
    }

private:
    static std::string parseString(const std::string_view &str);

    static std::string parsePreference(const OptionRawData &pref);

private:
    std::string p_;

    friend std::ostream &operator<<(std::ostream &os, const UiPath &path) {
        path.print(os);
        return os;
    }
};

inline void operator>>(const OptionRawData &data, UiPath &obj) {
    obj = data;
}

using UiPathVec = std::vector<UiPath>;

void operator>>(const OptionRawData &data, UiPathVec &vec);

}
