#pragma once

#include <string>
#include <QPixmap>

#include "util/actor.h"
#include "util/sqlite.h"

#include "img-loc.h"


namespace myapp {

class ImgCache : public actor::ThreadedActor {
public:

    class AddImgMsg : public actor::Message {
    public:

        AddImgMsg() {}

        AddImgMsg(const ImgLoc &loc, const QPixmap &img)
            : loc(loc)
            , img(img) {}

        ImgLoc loc;

        QPixmap img;
    };

    class LoadImgReq : public actor::Request {
    public:

        class Response : public actor::Response {
        public:
            Response() {}

            explicit Response(const QPixmap &img)
                : img(img) {}

            QPixmap img;
        };

        LoadImgReq() {}

        LoadImgReq(const ImgLoc &loc)
            : loc(loc) {}

        ImgLoc loc;
    };

public:
    ImgCache();

    virtual ~ImgCache() {}
};

}
