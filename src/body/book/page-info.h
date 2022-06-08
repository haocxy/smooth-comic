#pragma once

#include "core/ustr.h"
#include "core/basetype.h"


namespace myapp {

class PageInfo {
public:
    PageInfo() {}

    PageInfo(const u32str &entryPath, i32 width, i32 height)
        : entryPath(entryPath), width(width), height(height) {}


    PageInfo(const PageInfo &other) {
        *this = other;
    }

    PageInfo &operator=(const PageInfo &other) {
        if (this != &other) {
            entryPath = other.entryPath;
            width = other.width;
            height = other.height;
        }
        return *this;
    }

    u32str entryPath;
    i32 width{};
    i32 height{};
};

}
