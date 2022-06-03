#pragma once

#include <string>
#include <QPixmap>

#include "core/fs.h"

#include "util/qobject-actor.h"


namespace myapp {

class ImgCache : public QObjectActor {
    Q_OBJECT
public:

    class ImgKey {
    public:
        ImgKey() {}

        ImgKey(const fs::path &archiveFilePath, const std::u8string &entryPath)
            : archiveFilePath(archiveFilePath)
            , entryPath(entryPath) {}

        // 打包文件（压缩文件或PDF等含有多个图片的文件）在文件系统中的路径
        fs::path archiveFilePath;
        // 图片在打包文件中的路径
        std::u8string entryPath;
    };

    class AddImgMsg : public actor::Message {
    public:

        AddImgMsg() {}

        AddImgMsg(const ImgKey &key, const QPixmap &img)
            : key(key)
            , img(img) {}

        ImgKey key;

        QPixmap img;
    };

    class LoadImgReq : public actor::Request {
    public:

        class Response : public actor::Response {
        public:
            Response() {}

            Response(const ImgKey &key, const QPixmap &img)
                : key(key), img(img) {}

            ImgKey key;
            QPixmap img;
        };

        LoadImgReq() {}

        LoadImgReq(const ImgKey &key)
            : key(key) {}

        ImgKey key;
    };

public:
    explicit ImgCache(QObject *parent = 0);

    virtual ~ImgCache() {}
};

}
