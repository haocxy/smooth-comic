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

class OptionCenter
{
public:

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

    using AfterLoadCallback = std::function<void(const OptionRawData &)>;
    using AfterLoadCallbacks = std::vector<AfterLoadCallback>;

    static OptionCenter &instance();

    void set(const std::string &key, const std::string &val, OptionSource source);

    void addAfterLoadCallback(const std::string &key, AfterLoadCallback &&callback) {
        afterLoadCallbacks_[key].push_back(std::move(callback));
    }

    void publishAllOptions();

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
    std::map<std::string, AfterLoadCallbacks> afterLoadCallbacks_;
};

}
