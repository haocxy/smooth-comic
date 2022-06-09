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
#include "get-thumb-img-req.h"


namespace myapp {

class BookCache;

// 代表抽象的书
// GUI模块和非GUI模块的总界面
// GUI模块不关心下层的诸如缓存、加载等逻辑的细节
class Book : public QObjectActor {
    Q_OBJECT
public:

    explicit Book(Engine &engine, QObject *parent = nullptr);

    virtual ~Book();

    void open(const fs::path &archiveFile);

signals:
    void sigBookOpenStarted(const QString &archivePath);

    void sigPageLoaded(const QString &entryPath, i32 width, i32 height);

protected:
    void onNotice(actor::Notice &notice) override;

    void onRequest(actor::Request &req) override;

private:
    void asyncDeleteBookCache();

    void handlePageOpenedNotice(const BookCache::PageOpenedNotice &n);

    void handleGetThumbImgReq(GetThumbImgReq &req);

private:
    Engine &engine_;
    fs::path archiveFile_;
    DeclarePtr<BookCache> cache_;
};


}
