#include "thumb-cache.h"

#include "./sql/asset.thumb-img-table-create.sql.h"


namespace myapp {

ThumbCache::ThumbCache(const fs::path &dbFile)
    : dbFile_(dbFile)
{
    setActorName("ThumbCache");
}

void ThumbCache::onActorStarted()
{
    // TODO 开发完成前为了方便每次都删除数据文件
    fs::remove(dbFile_);

    db_.open(dbFile_);

    // 只在启动时执行一次，不需要保存statement对象
    sqlite::Statement stmtCreateTable(db_, reinterpret_cast<const char *>(Asset::Data::thumb_img_table_create__sql));
    stmtCreateTable.execute();
}

}
