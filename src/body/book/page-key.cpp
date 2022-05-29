#include "page-key.h"

#include <cctype>
#include <sstream>
#include <optional>


namespace myapp {

static std::u8string_view getName(const std::u8string_view &path)
{
    int32_t len = static_cast<int32_t>(path.size());

    for (int32_t i = len - 1; i >= 0; --i) {
        char8_t ch = path[i];
        if (ch == '/' || ch == '\\') {
            return path.substr(i + 1);
        }
    }
    return path;
}

static std::optional<PageNum> toPageNum(const std::string &s)
{
    std::istringstream ss(s);
    PageNum pageNum = 0;
    ss >> pageNum;

    // 如果转换失败，即使这个字符串都是由数字组成的，也认为是无效的，忽略它
    if (!ss.fail()) {
        return pageNum;
    } else {
        return std::nullopt;
    }
}

static std::vector<PageNum> mkParts(const std::u8string_view &path)
{
    std::vector<PageNum> parts;
    const std::u8string_view name = getName(path);
    std::string numbuff;

    for (char8_t ch8 : name) {
        char ch = ch8;
        if (std::isdigit(ch)) {
            numbuff.push_back(ch);
        } else {
            std::optional<PageNum> pageNumOpt = toPageNum(numbuff);
            if (pageNumOpt) {
                parts.push_back(*pageNumOpt);
            }
            numbuff.clear();
        }
    }

    if (!numbuff.empty()) {
        std::optional<PageNum> pageNumOpt = toPageNum(numbuff);
        if (pageNumOpt) {
            parts.push_back(*pageNumOpt);
        }
    }

    return parts;
}

PageKey::PageKey(const std::u8string_view &path)
    : parts_(mkParts(path))
{
}

}
