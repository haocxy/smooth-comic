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
// 其它缓存模块（例如缩略图缓存和图片缓存）都隐藏在这个模块的后面，上层UI逻辑只直接使用这个模块
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
            pages_.push_back(PageInfo(entryPath, width, height));
        }

        PageOpenedNotice(const PageOpenedNotice &other)
            : actor::Notice(other)
            , pages_(other.pages_) {}

        actor::Notice *clone() const override {
            return new PageOpenedNotice(*this);
        }

        std::vector<PageInfo> pages_;
    };

    BookCache(Engine &engine, const fs::path &archiveFile);

    virtual ~BookCache();

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

    class Page {
    public:
        u8str entryPath;
        i32 width;
        i32 height;
    };

    class StmtGetPages {
    public:
        StmtGetPages() {}

        void open(sqlite::Database &db);

        void reset() {
            stmt_.reset();
        }

        bool next();

        Page item();

    private:
        sqlite::Statement stmt_;
    };

private:
    Engine &engine_;
    const fs::path archiveFile_;
    CacheActorLogger logger_;
    sqlite::Database db_;
    StmtGetPages stmtGetPages_;
    DeclarePtr<PageLoader> loader_;
    DeclarePtr<ThumbCache> thumbCache_;
    DeclarePtr<PageCache> pageCache_;
};

}
