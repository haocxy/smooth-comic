#pragma once

#include <stdint.h>
#include <stdexcept>
#include <string>
#include <sstream>

#include "core/fs.h"
#include "core/strutil.h"
#include "option-source.h"


namespace debug_option {

class OptionRawData
{
public:

    OptionSource source() const {
        return source_;
    }

    void setSource(OptionSource source) {
        source_ = source;
    }

    const std::string &name() const {
        return name_;
    }

    void setName(const std::string &name) {
        name_ = name;
    }

    const std::string &value() const {
        return value_;
    }

    void setValue(const std::string &value) {
        value_ = value;
    }

    const fs::path &configFileDir() const {
        return configFileDir_;
    }

    void setConfigFileDir(const fs::path &dir) {
        configFileDir_ = dir;
    }

    const fs::path &procStartupDir() const {
        return procStartupDir_;
    }

    void setProcStartupDir(const fs::path &dir) {
        procStartupDir_ = dir;
    }

private:
    OptionSource source_ = OptionSource::Invalid;
    std::string name_;
    std::string value_;
    fs::path configFileDir_;
    fs::path procStartupDir_;
};

inline void operator>>(const OptionRawData &data, std::string &obj) {
    obj = data.value();
}

inline void operator>>(const OptionRawData &data, int16_t &obj) {
    std::istringstream(data.value()) >> obj;
}

inline void operator>>(const OptionRawData &data, uint16_t &obj) {
    std::istringstream(data.value()) >> obj;
}

inline void operator>>(const OptionRawData &data, int32_t &obj) {
    std::istringstream(data.value()) >> obj;
}

inline void operator>>(const OptionRawData &data, uint32_t &obj) {
    std::istringstream(data.value()) >> obj;
}

inline void operator>>(const OptionRawData &data, int64_t &obj) {
    std::istringstream(data.value()) >> obj;
}

inline void operator>>(const OptionRawData &data, uint64_t &obj) {
    std::istringstream(data.value()) >> obj;
}

inline void operator>>(const OptionRawData &data, bool &obj) {
    const std::string &s = data.value();
    if (strutil::equalIgnoreCase(s, "true")) {
        obj = true;
        return;
    }
    if (strutil::equalIgnoreCase(s, "false")) {
        obj = false;
        return;
    }
    if (s == "1") {
        obj = true;
        return;
    }
    if (s == "0") {
        obj = false;
        return;
    }

    // 如果没有明确的有效字符串，则什么都不做，使用默认值
}

}
