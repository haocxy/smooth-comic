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
{
    exec([this] {
        actor_ = std::make_unique<Actor>(*this);
    });
}

BookCache::~BookCache()
{
}

BookCache::Actor::Actor(BookCache &self)
    : self_(self)
{
    prepareDb();

    loader_ = std::make_unique<BookLoader>(self_.archiveFile_);

    loaderSigConns_.clear();

    loaderSigConns_ += loader_->sigPageLoaded.connect([this](sptr<LoadedPage> page) {
        self_.exec([this, page] { onPageLoaded(page); });
    });

    loaderSigConns_ += loader_->sigBookLoaded.connect([this]() {
        self_.exec([this] { onBookLoaded(); });
    });

    loader_->start();

    props_.setLoadStartTime(LoadClock::now());
}

void BookCache::Actor::prepareDb()
{
    fs::create_directories(self_.dbFile_.parent_path());

    db_.open(self_.dbFile_);

    db_.exec(kSqlCreateTables);

    props_.open(db_);
}

void BookCache::Actor::onPageLoaded(sptr<LoadedPage> page)
{
    gLogger.d << "onPageLoaded: " << page->name;
}

void BookCache::Actor::onBookLoaded()
{
    gLogger.d << "onBookLoaded";

    props_.setLoadSucceedTime(LoadClock::now());
}

void BookCache::Props::open(sqlite::Database &db)
{
    propRepo_.open(db, "book_props");
}

}
