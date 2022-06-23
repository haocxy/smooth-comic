#include "option-center.h"

#include <sstream>
#include <fstream>
#include <boost/algorithm/string/trim.hpp>

#include "core/strutil.h"
#include "core/logger.h"
#include "core/fs.h"
#include "core/proc-startup-info.h"

#include "option-convert.h"


namespace {

static const char *title() {
    return "OptionCenter ";
}

using logger::gLogger;

}

namespace debug_option {

OptionCenter &OptionCenter::instance() {
    static OptionCenter s_instance;
    return s_instance;
}

void OptionCenter::set(const std::string &key, const std::string &val, OptionSource source) {
    gLogger.d << title() << "set [" << key << "] to [" << val << "]";
    Data data;
    data.setSource(source);
    data.setContent(val);
    map_[key] = data;
    nameToValue_[key] = val;
}


void OptionCenter::loadCfgFile(const std::string &path) {

    configFileDir_ = fs::path(path).parent_path();

    gLogger.d << title() << "loadCfgFile: [" << path << "]";

    const fs::path abs = fs::absolute(path).lexically_normal();
    if (!fs::exists(abs)) {
        std::ostringstream ss;
        ss << "Config file '" << path << "' not exist";
        throw ConfigFileError(ss.str().c_str());
    }

    if (fs::is_directory(abs)) {
        std::ostringstream ss;
        ss << "Config file path '" << path << "' is a directory";
        throw ConfigFileError(ss.str().c_str());
    }

    std::ifstream ifs(abs.generic_string());
    if (!ifs) {
        std::ostringstream ss;
        ss << "Cannot read config file '" << path << "'";
        throw ConfigFileError(ss.str().c_str());
    }

    gLogger.d << title() << "loadCfgFile cfg file opened: [" << abs.generic_string() << "]";
    std::string line;
    while (std::getline(ifs, line)) {
        gLogger.d << title() << "loadCfgFile line: [" << line << "]";
        const auto noteSepPos = line.rfind('#');
        if (noteSepPos != std::string::npos) {
            line = line.substr(0, noteSepPos);
        }
        boost::trim(line);
        if (line.empty()) {
            continue;
        }
        const auto kvSepPos = line.find(':');
        if (kvSepPos == std::string::npos) {
            continue;
        }
        std::string key = line.substr(0, kvSepPos);
        boost::trim(key);
        std::string val = line.substr(kvSepPos + 1);
        boost::trim(val);
        loadCfgFilePair(key, val);
    }
}

void OptionCenter::loadCfgFilePair(const std::string &name, const std::string &value) {
    set(name, parseValue(value), OptionSource::ConfigFile);
}

std::string OptionCenter::parseValue(const std::string &str) const {
    return strutil::interpolate(str, nameToValue_);
}

void OptionCenter::loadProcArgs(int argc, char *argv[]) {
    gLogger.d << title() << "loadProcArgs";
}

void OptionCenter::publishAllOptions() {
    gLogger.d << title() << "afterLoad";

    for (auto &[name, callbacks] : afterLoadCallbacks_) {
        for (auto &callback : callbacks) {
            const auto it = map_.find(name);
            if (it != map_.end()) {
                OptionRawData data;
                data.setSource(it->second.source());
                data.setName(name);
                data.setValue(it->second.content());
                data.setConfigFileDir(configFileDir_);
                data.setProcStartupDir(proc_startup_info::dir());
                callback(data);
            } else {
                gLogger.d << title() << "afterLoad, no value found for [" << name << "]";
            }
        }
    }
}

}
