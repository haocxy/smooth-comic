#include "thumb-cache.h"

#include <QByteArray>
#include <QBuffer>

#include "core/strutil.h"
#include "engine/engine.h"
#include "engine/path-manager.h"
#include "util/img-util.h"

#include "./sql/asset.thumb-cache-table-create.sql.h"


namespace myapp {

static logger::LogStream &operator<<(logger::LogStream &stream, const ThumbCache::AddPageThumbMsg &m)
{
    stream << "ThumbCache::AddPageThumbMsg(" << reinterpret_cast<const char *>(m.entryPath.c_str()) << ")";
    return stream;
}

static const std::string kTableName{ "thumb_cache" };

static const int kThumbImgWidth = 400;
static const char *const kThumbImgFormat = "WEBP";

ThumbCache::ThumbCache(Engine &engine, const fs::path &archiveFile)
    : engine_(engine)
    , archiveFile_(archiveFile)
    , logger_("ThumbCache", archiveFile_)
{
    setActorName("ThumbCache");
}

ThumbCache::~ThumbCache()
{
    stopAndJoin();

    logger_.d << "destructed";
}

void ThumbCache::onActorStarted()
{
    db_.open(engine_.pathManager().mkThumbCacheDbFilePath(archiveFile_));
    logger_.d << "db opend: " << db_.path();

    ensureTableExist();

    if (shouldClearDb()) {
        clearDb();
    }

    stmtSaveItem_.open(db_);
}

void ThumbCache::onMessage(actor::Message &msg)
{
    if (AddPageThumbMsg *m = msg) {
        return handleAddPageThumbMsg(*m);
    }
}

void ThumbCache::ensureTableExist()
{
    createTable();
}

bool ThumbCache::shouldClearDb() const
{
    return fs::last_write_time(archiveFile_) > fs::last_write_time(db_.path());
}

void ThumbCache::clearDb()
{
    removeTable();
    createTable();
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

void ThumbCache::handleAddPageThumbMsg(AddPageThumbMsg &m)
{
    logger_.d << "handle " << m;

    QPixmap thumbImg = m.rawImg.scaledToWidth(kThumbImgWidth);

    QByteArray bytes = ImgUtil::toByteArray(thumbImg, kThumbImgFormat);

    stmtSaveItem_(m.entryPath, bytes.constData(), bytes.length());
}

void ThumbCache::StmtSaveItem::open(sqlite::Database &db)
{
    stmt_.open(db, std::format("insert or replace into {} values(?,?);", kTableName));
}

void ThumbCache::StmtSaveItem::operator()(const u8str &entryPath, const void *data, size_t len)
{
    stmt_.reset();
    stmt_.arg(entryPath);
    stmt_.arg(data, len);
    stmt_.execute();
}

}
