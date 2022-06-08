#include "thumb-cache.h"

#include "core/strutil.h"
#include "engine/engine.h"
#include "engine/path-manager.h"

#include "./sql/asset.thumb-cache-table-create.sql.h"


namespace myapp {

ThumbCache::ThumbCache(Engine &engine, const fs::path &archiveFile)
    : engine_(engine)
    , archiveFile_(archiveFile)
{
    setActorName("ThumbCache");
}

void ThumbCache::onActorStarted()
{
    db_.open(engine_.pathManager().mkThumbCacheDbFilePath(archiveFile_));

    ensureTableExist();
}

void ThumbCache::ensureTableExist()
{
    db_.exec(Asset::StrView::thumb_cache_table_create__sql);
}

}
