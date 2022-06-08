#include "book-cache.h"

#include "core/logger.h"
#include "engine/engine.h"
#include "engine/path-manager.h"

#include "page-loader.h"
#include "thumb-cache.h"
#include "page-cache.h"

#include "./sql/asset.book-cache-table-create.sql.h"


namespace myapp {

using logger::gLogger;

static logger::LogStream &operator<<(logger::LogStream &stream, const myapp::PageLoader::PageLoadedMsg &m) {
    stream << "PageLoader::PageLoadedMsg(entry:" << reinterpret_cast<const char *>(m.entryPath.c_str()) << ",w:" << m.img.width() << ",h:" << m.img.height() << ")";
    return stream;
}


static const std::string kTableName{ "book_cache" };

BookCache::BookCache(Engine &engine, const fs::path &archiveFile)
    : engine_(engine)
    , archiveFile_(archiveFile)
    , logger_("BookCache", archiveFile_)
{
    setActorName("BookCache");

    loader_ = new PageLoader(archiveFile_);
    thumbCache_ = new ThumbCache(engine_, archiveFile_);
    pageCache_ = new PageCache(engine_.pathManager().mkPageCacheDbFilePath(archiveFile_));

    logger_.d << "constructed";
}

BookCache::~BookCache()
{
    stopEventHandle();
}

void BookCache::onActorStarted()
{
    db_.open(engine_.pathManager().mkBookCacheDbFilePath(archiveFile_));
    logger_.d << "db opened: " << db_.path();

    ensureTableExist();

    stmtGetPages_.open(db_);
}

void BookCache::onMessage(actor::Message &msg)
{
    if (OpenBookMsg *m = msg) {
        return onOpenBookMsg(*m);
    }

    if (PageLoader::PageLoadedMsg *m = msg) {
        return onPageLoadedMsg(*m);
    }
}

void BookCache::onOpenBookMsg(OpenBookMsg &m)
{
    logger_.d << "on OpenBookMsg";

    if (shouldClearDb()) {
        clearDb();
    }

    if (shouldLoadFromArchive()) {
        sendTo(*loader_, new PageLoader::LoadFromArchieMsg);
    }
}

void BookCache::onPageLoadedMsg(PageLoader::PageLoadedMsg &m)
{
    logger_.d << m;

    sendTo(*thumbCache_, new ThumbCache::AddPageThumbMsg(m.entryPath, m.img));

    notify(std::make_unique<PageOpenedNotice>(m.entryPath, m.img.width(), m.img.height()));
}

void BookCache::removeTable()
{
    db_.exec(std::format("drop table {};", kTableName));
    logger_.d << "table removed";
}

void BookCache::ensureTableExist()
{
    createTable();
}

void BookCache::createTable()
{
    db_.exec(Asset::StrView::book_cache_table_create__sql);
    logger_.d << "table created";
}

void BookCache::clearDb()
{
    removeTable();
    createTable();
}

bool BookCache::shouldClearDb() const
{
    return fs::last_write_time(archiveFile_) > fs::last_write_time(db_.path());;
}

bool BookCache::shouldLoadFromArchive()
{
    stmtGetPages_.reset();
    return !stmtGetPages_.next();
}

void BookCache::StmtGetPages::open(sqlite::Database &db)
{
    stmt_.open(db, std::format("select * from {};", kTableName));
}

bool BookCache::StmtGetPages::next()
{
    return stmt_.nextRow();
}

BookCache::Page BookCache::StmtGetPages::item()
{
    Page p;
    stmt_.getValue(0, p.entryPath);
    stmt_.getValue(1, p.width);
    stmt_.getValue(2, p.height);

    return p;
}

}
