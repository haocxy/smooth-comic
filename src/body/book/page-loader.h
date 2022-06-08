#pragma once

#include <QPixmap>

#include "core/fs.h"
#include "util/actor.h"
#include "page-num.h"


namespace myapp {

// 用于从指定的数据源加载图片
// 一个该类的对象对应一本书
class PageLoader : public actor::ThreadedActor {
public:

    class StartLoadMsg : public actor::Message {
    public:
        StartLoadMsg(const fs::path &archive) : archive(archive) {}

        fs::path archive;
    };

    class PageLoadedMsg : public actor::Message {
    public:
        PageLoadedMsg(PageNum pageNum, const QPixmap &img)
            : pageNum(pageNum), img(img) {}

        PageNum pageNum = 0;
        QPixmap img;
    };

    PageLoader(const fs::path &archiveFile);

    virtual ~PageLoader();

protected:
    virtual void onMessage(actor::Message &msg) override;

private:

    void doStartLoadFromLocalFile(actor::WeakHandle peer, const fs::path &archiveFile);

private:
    const fs::path archiveFile_;
};

}
