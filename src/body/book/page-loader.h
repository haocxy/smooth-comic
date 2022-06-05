#pragma once

#include <QPixmap>

#include "core/fs.h"
#include "util/actor.h"
#include "page-num.h"


namespace myapp {

class BookLoader : public actor::ThreadedActor {
public:

    class StartLoadMsg : public actor::Message {
    public:
        StartLoadMsg(const fs::path &archive) : archive(archive) {}

        fs::path archive;
    };

    class PageLoaded : public actor::Notice {
    public:
        PageLoaded() {}

        PageLoaded(PageNum pageNum, const QPixmap &img)
            : pageNum(pageNum), img(img) {}

        PageLoaded(const PageLoaded &other)
            : actor::Notice(other), pageNum(other.pageNum), img(other.img) {}

        virtual actor::Notice *clone() const override {
            return new PageLoaded(pageNum, img);
        }

        PageNum pageNum = 0;
        QPixmap img;
    };

    BookLoader();

    virtual ~BookLoader();

protected:
    virtual void onMessage(actor::Message &msg) override;

private:

    void doStartLoadFromLocalFile(const fs::path &archiveFile);
};

}
