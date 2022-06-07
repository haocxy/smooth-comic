#include "thumb-cache.h"

#include "./sql/asset.thumb-cache-table-create.sql.h"

#include "core/strutil.h"


namespace myapp {

ThumbCache::ThumbCache(const fs::path &dbFile)
    : dbFile_(dbFile)
{
    setActorName("ThumbCache");
}

void ThumbCache::onActorStarted()
{
    db_.open(dbFile_);

    // 只在启动时执行一次，不需要保存statement对象
    sqlite::Statement stmtCreateTable(db_, strutil::interpolate(Asset::StrView::thumb_cache_table_create__sql, "name", "abc"));
    stmtCreateTable.execute();
}

}
