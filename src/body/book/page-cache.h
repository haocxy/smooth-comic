#pragma once

#include <string>
#include <QPixmap>

#include "core/fs.h"
#include "util/actor.h"
#include "util/sqlite.h"

#include "img-loc.h"


namespace myapp {

// 用于缓存原始尺寸的图片
// 缓存的图片可能有和原始包中不同的格式，但一定有相同的图片信息
// 一个该类的对象对应一本书
class PageCache : public actor::ThreadedActor {
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
    PageCache(const fs::path &dbFile);

    virtual ~PageCache() {}

private:
    const fs::path dbFile_;
};

}