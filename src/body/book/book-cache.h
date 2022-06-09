#pragma once

#include <vector>

#include "core/fs.h"
#include "core/ustr.h"
#include "core/logger.h"
#include "core/declare_ptr.h"
#include "util/actor.h"
#include "util/sqlite.h"

#include "page-loader.h"
#include "cache-actor-logger.h"
#include "page-info.h"


namespace myapp {

class Engine;
class ThumbCache;
class PageCache;


// 整个缓存系统的对外接口
class BookCache : public actor::ThreadedActor {
public:

    class OpenBookMsg : public actor::Message {
    public:
        OpenBookMsg() {}
    };

    class PageOpenedNotice : public actor::Notice {
    public:
        PageOpenedNotice() {}

        PageOpenedNotice(const u32str &entryPath, int width, int height) {
            pages.push_back(PageInfo(entryPath, width, height));
        }

        PageOpenedNotice(const PageInfo &page) {
            pages.push_back(page);
        }

        PageOpenedNotice(const PageOpenedNotice &other)
            : actor::Notice(other)
            , pages(other.pages) {}

        actor::Notice *clone() const override {
            return new PageOpenedNotice(*this);
        }

        std::vector<PageInfo> pages;
    };

    BookCache(Engine &engine, const fs::path &archiveFile);

    virtual ~BookCache();

    ThumbCache &thumbCache() {
        return *thumbCache_;
    }

protected:
    void onActorStarted() override;

    void onMessage(actor::Message &msg) override;


private:

    void onOpenBookMsg(OpenBookMsg &m);

    void onPageLoadedMsg(PageLoader::PageLoadedMsg &m);

    void removeTable();

    void ensureTableExist();
    
    void createTable();

    void clearDb();

    bool shouldClearDb() const;

    bool shouldLoadFromArchive();

    class StmtGetPages {
    public:
        StmtGetPages() {}

        void open(sqlite::Database &db);

        void reset() {
            stmt_.reset();
        }

        bool next();

        PageInfo item();

    private:
        sqlite::Statement stmt_;
    };

    class StmtSavePage {
    public:
        StmtSavePage() {}

        void open(sqlite::Database &db);

        void reset() {
            stmt_.reset();
        }

        void operator()(const PageInfo &page);

    private:
        sqlite::Statement stmt_;
    };

private:
    Engine &engine_;
    const fs::path archiveFile_;
    CacheActorLogger logger_;
    sqlite::Database db_;
    StmtGetPages stmtGetPages_;
    StmtSavePage stmtSavePage_;
    DeclarePtr<PageLoader> loader_;
    DeclarePtr<ThumbCache> thumbCache_;
    DeclarePtr<PageCache> pageCache_;
};

}
