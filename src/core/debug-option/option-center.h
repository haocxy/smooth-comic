#pragma once

#include <map>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>

#include "core/fs.h"
#include "option-source.h"


namespace debug_option {

class OptionRawData;

class OptionInfo {
public:
    using AfterLoadCallback = std::function<void(const OptionRawData &)>;

    OptionInfo() {}

    OptionInfo(std::string &&type, AfterLoadCallback &&cb, std::string &&desc)
        : type(std::move(type)), cb(std::move(cb)), desc(std::move(desc)) {}

    std::string type;
    AfterLoadCallback cb;
    std::string desc;
};

class OptionCenter
{
public:

    using AfterLoadCallback = OptionInfo::AfterLoadCallback;

    class ConfigFileError : public std::runtime_error
    {
    public:
        ConfigFileError(const char *msg) : std::runtime_error(msg) {}
        virtual ~ConfigFileError() {}
    };

    class Data
    {
    public:
        OptionSource source() const {
            return source_;
        }

        void setSource(OptionSource source) {
            source_ = source;
        }

        const std::string &content() const {
            return content_;
        }

        void setContent(const std::string &content) {
            content_ = content;
        }

    private:
        OptionSource source_ = OptionSource::Invalid;
        std::string content_;
    };

    

    static OptionCenter &instance();

    void set(const std::string &key, const std::string &val, OptionSource source);

    void addOption(const std::string &name, std::string &&type, AfterLoadCallback &&callback, std::string &&desc) {
        if (options_.contains(name)) {
            throw std::logic_error(std::format("OptionCenter::addOption(...) option name [{}] already used", name));
        }
        options_[name] = OptionInfo(std::move(type), std::move(callback), std::move(desc));
    }

    void publishAllOptions();

    void eachOption(std::function<void(const std::string &name, const OptionInfo &optionInfo)> &&action) {
        for (const auto &pair : options_) {
            action(pair.first, pair.second);
        }
    }

private:
    

    OptionCenter() {}


    void loadCfgFile(const std::string &path);

    void loadCfgFilePair(const std::string &name, const std::string &value);

    std::string parseValue(const std::string &str) const;

    void loadProcArgs(int argc, char *argv[]);



private:
    fs::path configFileDir_;
    std::map<std::string, std::string> nameToValue_;
    std::map<std::string, Data> map_;
    std::map<std::string, OptionInfo> options_;
};

}
