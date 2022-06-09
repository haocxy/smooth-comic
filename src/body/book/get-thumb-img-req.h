#pragma once

#include <QPixmap>

#include "core/ustr.h"
#include "util/actor.h"


namespace myapp {

class GetThumbImgReq : public actor::Request {
public:

    GetThumbImgReq() {}

    GetThumbImgReq(const u8str &entryPath)
        : entryPath(entryPath) {}

    GetThumbImgReq(const GetThumbImgReq &other)
        : entryPath(other.entryPath) {}

    GetThumbImgReq &operator=(const GetThumbImgReq &other) {
        actor::Request::operator=(other);
        entryPath = other.entryPath;
        return *this;
    }

    class Response : public actor::Response {
    public:
        Response() {}

        Response(const QPixmap &img)
            : img(img) {}

        QPixmap img;
    };

    u8str entryPath;
};

}
