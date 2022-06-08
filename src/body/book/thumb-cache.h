#pragma once

#include <QPixmap>

#include "util/actor.h"
#include "util/sqlite.h"

#include "cache-actor-logger.h"


namespace myapp {

class Engine;

// 缩略图缓存
// 一个该类的对象对应一本书
class ThumbCache : public actor::ThreadedActor {
public:

    class AddPageThumbMsg : public actor::Message {
    public:
        AddPageThumbMsg() {}

        AddPageThumbMsg(const u8str &entryPath, const QPixmap &rawImg)
            : entryPath(entryPath), rawImg(rawImg) {}

        u8str entryPath;
        QPixmap rawImg;
    };

    ThumbCache(Engine &engine, const fs::path &archiveFile);

    virtual ~ThumbCache();

protected:
    void onActorStarted() override;

    void onMessage(actor::Message &msg) override;

private:
    void ensureTableExist();

    bool shouldClearDb() const;

    void clearDb();

    void removeTable();

    void createTable();

    void handleAddPageThumbMsg(AddPageThumbMsg &m);

    class StmtSaveItem {
    public:
        StmtSaveItem() {}

        void open(sqlite::Database &db);

        void operator()(const u8str &entryPath, const void *data, size_t len);

    private:
        sqlite::Statement stmt_;
    };

private:
    Engine &engine_;
    const fs::path archiveFile_;
    CacheActorLogger logger_;
    sqlite::Database db_;
    StmtSaveItem stmtSaveItem_;
};


}

