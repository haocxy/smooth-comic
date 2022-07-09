#pragma once

#include <QString>


namespace myapp {

class FontIcon {
public:
    FontIcon() {}

    template <typename CodeType>
    FontIcon(const QString &fontFamily, CodeType code)
        : fontFamily_(fontFamily), code_(static_cast<char16_t>(code)) {

        static_assert(sizeof(CodeType) <= sizeof(char16_t));
    }

    const QString &fontFamily() const {
        return fontFamily_;
    }

    void setFontFamily(const QString &fontFamily) {
        fontFamily_ = fontFamily;
    }

    char16_t code() const {
        return code_;
    }

    void setCode(char16_t code) {
        code_ = code;
    }

private:
    QString fontFamily_;
    char16_t code_{};
};

}
