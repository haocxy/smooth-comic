#pragma once

#include <map>

#include <QObject>
#include <QHash>
#include <QPixmap>

#include "core/fs.h"
#include "core/declare_ptr.h"
#include "engine/engine.h"
#include "util/qobject-actor.h"

#include "page-num.h"


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
    void sigPageLoaded(PageNum pageNum, const QPixmap &img, PageNum totalPages);

private:
    void handleOnPageLoaded(PageNum pageNum, const QPixmap &img);

private:
    Engine &engine_;
    const fs::path archiveFile_;
    DeclarePtr<class BookCache> cache_;
    std::map<PageNum, QPixmap> pageKeyToImg_;
};


}
