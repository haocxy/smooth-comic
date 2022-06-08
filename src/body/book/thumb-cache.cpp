#include "thumb-cache.h"

#include "core/strutil.h"
#include "engine/engine.h"
#include "engine/path-manager.h"

#include "./sql/asset.thumb-cache-table-create.sql.h"


namespace myapp {


static const std::string kTableName{ "thumb_cache" };

ThumbCache::ThumbCache(Engine &engine, const fs::path &archiveFile)
    : engine_(engine)
    , archiveFile_(archiveFile)
    , logger_("ThumbCache", archiveFile_)
{
    setActorName("ThumbCache");
}

ThumbCache::~ThumbCache()
{
    stopEventHandle();
}

void ThumbCache::onActorStarted()
{
    db_.open(engine_.pathManager().mkThumbCacheDbFilePath(archiveFile_));
    logger_.d << "db opend: " << db_.path();

    ensureTableExist();
}

void ThumbCache::ensureTableExist()
{
    createTable();
}

bool ThumbCache::shouldClearDb() const
{
    return fs::last_write_time(archiveFile_) > fs::last_write_time(db_.path());
}

void ThumbCache::removeTable()
{
    db_.exec(std::format("drop table {};", kTableName));
    logger_.d << "table removed";
}

void ThumbCache::createTable()
{
    db_.exec(Asset::StrView::thumb_cache_table_create__sql);
    logger_.d << "table created";
}

}
