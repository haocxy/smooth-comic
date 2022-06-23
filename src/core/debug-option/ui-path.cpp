#include "ui-path.h"

#include <cassert>
#include <cstdlib>
#include <cctype>

#include "core/fs.h"
#include "core/strutil.h"
#include "core/system.h"


namespace debug_option {

static std::string_view trim(const std::string_view &str) {
    const std::string_view::size_type len = str.length();
    std::string_view::size_type left = 0;
    for (; left < len; ++left) {
        if (!std::isblank(str[left])) {
            break;
        }
    }
    std::string_view::size_type right = len - 1;
    for (; right > left && right < len; --right) {
        if (!std::isblank(str[right])) {
            break;
        }
    }
    return str.substr(left, right - left + 1);
}

static std::string replace(const std::string_view &str, char from, char to) {
    std::string result;
    for (char c : str) {
        if (c == from) {
            result.push_back(to);
        } else {
            result.push_back(c);
        }
    }
    return result;
}

static bool startWith(const std::string_view &str, const std::string_view &prefix) {
    const std::string_view::size_type prefixLen = prefix.length();
    if (str.length() < prefixLen) {
        return false;
    }
    for (std::string_view::size_type i = 0; i < prefixLen; ++i) {
        if (str[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

static std::string homePath() {
    return SystemUtil::userHome().generic_string();
}

static const std::string_view kHomePathPrefix("~/");

static std::string tryParseUserHomePath(const std::string_view &str) {
    if (str == "~") {
        return std::string(trim(homePath()));
    }

    if (startWith(str, kHomePathPrefix)) {
        const std::string_view relativePart = str.substr(kHomePathPrefix.size());
        std::string result(trim(homePath()));
        result.push_back('/');
        result.append(trim(relativePart));
        return result;
    }

    return std::string(str);
}

static std::string trimAndReplace(const std::string_view &str) {
    const std::string_view trimed = trim(str);
    return replace(trimed, '\\', '/');
}

std::string UiPath::parseString(const std::string_view &str) {
    const std::string homeParsed = tryParseUserHomePath(trimAndReplace(str));

    return fs::absolute(homeParsed).lexically_normal().generic_string();
}

std::string UiPath::parsePreference(const OptionRawData &pref) {
    const std::string trimAndReplaced = trimAndReplace(pref.value());
    const fs::path p(trimAndReplaced);
    if (p.is_absolute()) {
        return p.lexically_normal().generic_string();
    }

    const std::string homeParsed = tryParseUserHomePath(trimAndReplaced);
    const fs::path homeParsedPath(homeParsed);
    if (homeParsedPath.is_absolute()) {
        return homeParsedPath.lexically_normal().generic_string();
    }

    if (pref.source() == OptionSource::ConfigFile) {
        return (pref.configFileDir() / homeParsed).lexically_normal().generic_string();
    } else if (pref.source() == OptionSource::CmdLine) {
        return (pref.procStartupDir() / homeParsed).lexically_normal().generic_string();
    } else {
        throw std::logic_error("invalid preference source type");
    }
}

void operator>>(const OptionRawData &data, UiPathVec &vec)
{
    assert(vec.empty());

    strutil::strviews views = strutil::split(data.value(), ':');
    for (const std::string_view &view : views) {
        OptionRawData partData;
        partData.setSource(data.source());
        partData.setName(data.name());
        partData.setValue(std::string(view));
        partData.setConfigFileDir(data.configFileDir());
        partData.setProcStartupDir(data.procStartupDir());

        UiPath path;
        path = partData;
        vec.push_back(std::move(path));
    }
}

}
