#pragma once

#include <map>

#include <QObject>
#include <QHash>
#include <QPixmap>

#include "core/fs.h"
#include "core/basetype.h"
#include "core/declare_ptr.h"
#include "engine/engine.h"
#include "util/qobject-actor.h"

#include "page-num.h"
#include "book-cache.h"


namespace myapp {

class BookCache;

// 代表抽象的书
// GUI模块和非GUI模块的总界面
// GUI模块不关心下层的诸如缓存、加载等逻辑的细节
class Book : public QObjectActor {
    Q_OBJECT
public:

    class GetThumbImgReq : public actor::Request {
    public:

        GetThumbImgReq() {}

        GetThumbImgReq(const u8str &entryPath)
            : entryPath(entryPath) {}

        class Response : public actor::Response {
        public:
            Response() {}

            Response(const QPixmap &img)
                : img(img) {}

            QPixmap img;
        };

        u8str entryPath;
    };

    explicit Book(Engine &engine, QObject *parent = nullptr);

    virtual ~Book();

    void open(const fs::path &archiveFile);

signals:
    void sigPageLoaded(const QString &entryPath, i32 width, i32 height);

protected:
    void onNotice(actor::Notice &notice) override;

private:
    void handlePageOpenedNotice(const BookCache::PageOpenedNotice &n);

private:
    Engine &engine_;
    const fs::path archiveFile_;
    DeclarePtr<class BookCache> cache_;
};


}
