#pragma once

#include <QPixmap>

#include "core/fs.h"
#include "core/ustr.h"
#include "util/actor.h"
#include "page-num.h"


namespace myapp {

// 用于从指定的数据源加载图片
// 一个该类的对象对应一本书
class PageLoader : public actor::ThreadedActor {
public:

    class LoadFromArchieMsg : public actor::Message {};

    class PageLoadedMsg : public actor::Message {
    public:
        PageLoadedMsg(const u8str &entryPath, const QPixmap &img)
            : entryPath(entryPath), img(img) {}

        u8str entryPath;
        QPixmap img;
    };

    PageLoader(const fs::path &archiveFile);

    virtual ~PageLoader();

protected:
    virtual void onMessage(actor::Message &msg) override;

private:

    void handleLoadFromArchieMsg(actor::WeakHandle peer);

private:
    const fs::path archiveFile_;
};

}
