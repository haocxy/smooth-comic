#include "book-cache.h"

#include "core/logger.h"


namespace myapp {

using logger::gLogger;

static const char *const kSqlCreateTable = R"(
create table if not exists page (
    seqNum int primary key,
    name text not null,
    rawWidth int not null,
    rawHeight int not null,
    rawImg blob not null,
    scaledImg blob not null
)
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
        gLogger.d << "loader say: sigPageLoaded, " << page->seqNum << ", " << static_cast<std::string>(u8str(page->name));
        });

    loaderSigConns_ += loader_->sigBookLoaded.connect([this]() {
        gLogger.d << "loader say: sigBookLoaded";
    });

    loader_->start();
}

void BookCache::Actor::prepareDb()
{
    fs::create_directories(self_.dbFile_.parent_path());


}

}
