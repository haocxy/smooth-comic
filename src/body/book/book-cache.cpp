#include "book-cache.h"

#include "core/logger.h"


namespace myapp {

using logger::gLogger;

static const char *const kSqlCreateTables = R"(
create table if not exists pages (
    seqNum int primary key,
    name text not null,
    rawWidth int not null,
    rawHeight int not null,
    rawImg blob not null,
    scaledImg blob not null
);
)";

BookCache::BookCache(const fs::path &archiveFile, const fs::path &dbFile)
    : SingleThreadStrand("BookCache")
    , archiveFile_(archiveFile)
    , dbFile_(dbFile)
    , actor_(*this)
{
}

BookCache::~BookCache()
{
    stopEventQueue();

    gLogger.d << "BookCache::~BookCache() end";
}

BookCache::Actor::Actor(BookCache &outer)
    : outer_(outer)
    , handle_(*this)
{
    gLogger.d << "BookCache::Actor::Actor() begin";

    prepareDb();

    loader_ = std::make_unique<BookLoader>(outer_.archiveFile_);

    loaderSigConns_.clear();

    loaderSigConns_ += loader_->sigPageLoaded.connect([this, h = handle_.weak()](sptr<LoadedPage> page) {
        h.apply([this, page] {
            outer_.exec([this, page] {
                onPageLoaded(page);
            });
        });
    });

    loaderSigConns_ += loader_->sigBookLoaded.connect([this, h = handle_.weak()](i32 totalPageCount) {
        h.apply([this, totalPageCount] {
            outer_.exec([this, totalPageCount] {
                onBookLoaded(totalPageCount);
            });
        });
    });

    loader_->start();

    props_.setLoadStartTime(LoadClock::now());
}

BookCache::Actor::~Actor()
{



    gLogger.d << "BookCache::Actor::~Actor() end";
}

void BookCache::Actor::prepareDb()
{
    fs::create_directories(outer_.dbFile_.parent_path());

    db_.open(outer_.dbFile_);

    db_.exec(kSqlCreateTables);

    props_.open(db_);

    stmtSavePage_.open(db_);
}

void BookCache::Actor::onPageLoaded(sptr<LoadedPage> page)
{
    gLogger.d << "onPageLoaded: " << page->name;

    // save data to db
    PageDbData data;
    data.seqNum = page->seqNum;
    data.name = page->name;
    data.rawWidth = page->rawWidth;
    data.rawHeight = page->rawHeight;
    data.rawImg = std::move(page->encodedRawImg);
    data.scaledImg = std::move(page->encodedScaledImg);

    stmtSavePage_(data);


    // emit signal
    PageInfo pageInfo;
    pageInfo.seqNum = page->seqNum;
    pageInfo.name = page->name;
    pageInfo.rawWidth = page->rawWidth;
    pageInfo.rawHeight = page->rawHeight;

    outer_.sigPageLoaded(pageInfo);
}

void BookCache::Actor::onBookLoaded(i32 totalPageCount)
{
    gLogger.d << "onBookLoaded";

    props_.setLoadSucceedTime(LoadClock::now());
    props_.setTotalPageCount(totalPageCount);
}

void BookCache::Props::open(sqlite::Database &db)
{
    propRepo_.open(db, "book_props");
}

void BookCache::Actor::StmtSavePage::open(sqlite::Database &db)
{
    stmt_.open(db, "insert or replace into pages values (?,?,?,?,?,?);");
}

void BookCache::Actor::StmtSavePage::operator()(const PageDbData &page)
{
    stmt_.reset();

    stmt_.arg(page.seqNum);
    stmt_.arg(page.name);
    stmt_.arg(page.rawWidth);
    stmt_.arg(page.rawHeight);
    stmt_.arg(page.rawImg.data(), page.rawImg.size());
    stmt_.arg(page.scaledImg.data(), page.scaledImg.size());

    stmt_.execute();
}

}
