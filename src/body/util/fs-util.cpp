#include "fs-util.h"


namespace myapp::FsUtil {

static std::u32string encode(const std::u32string &path) {
    std::u32string result;
    for (char32_t c : path) {
        if (c == '/') {
            result.push_back('-');
            result.push_back('L');
        } else if (c == '\\') {
            result.push_back('-');
            result.push_back('R');
        } else if (c == ':') {
            result.push_back('-');
            result.push_back('W');
        } else if (c == '-') {
            result.push_back('-');
            result.push_back('A');
        } else if (c >= 0x80) {
            result.push_back('-');
            result.push_back('U[');
            std::ostringstream ss;
            ss << std::hex << static_cast<uint32_t>(c);
            for (char ch : ss.str()) {
                result.push_back(ch);
            }
            result.push_back(']');
        } else {
            result.push_back(c);
        }
    }
    return result;
}

std::u32string encodePathToName(const fs::path &path)
{
    return encode(path.generic_u32string());
}

}
