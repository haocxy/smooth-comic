#include "book-cache.h"

#include <QPixmap>

#include "core/logger.h"
#include "core/debug-option.h"

namespace myapp {

using logger::gLogger;


static DebugOption<bool> dopLog("log.need.book-cache", false,
    "Is log for book cache needed?");


static const char *const kSqlCreateTables = R"(
create table if not exists pages (
    seqNum int primary key,
    name text not null,
    rawWidth int not null,
    rawHeight int not null,
    rawImg blob,
    scaledImg blob not null
);
)";

BookCache::BookCache(const OpenSessionId &sessionId, const fs::path &archiveFile, const fs::path &dbFile, ShouldForceReload shouldForceReload)
    : PrioritySingleThreadStrand<BookOperationPriority>("BookCache")
    , sessionId_(sessionId)
    , archiveFile_(archiveFile)
    , dbFile_(dbFile)
    , shouldForceReload_(shouldForceReload)
{
    exec([this] {
        actor_ = std::make_unique<Actor>(*this);
    }, Prio::Init);
}

BookCache::~BookCache()
{
    std::mutex mtxStopped;
    std::condition_variable cvStopped;

    std::unique_lock<std::mutex> lockStopped(mtxStopped);

    exec([this, &cvStopped] {
        actor_ = nullptr;
        stopEventQueue();
        cvStopped.notify_all();
    }, Prio::DeInit);

    cvStopped.wait(lockStopped);

    if (*dopLog) {
        gLogger.d << "BookCache::~BookCache() end";
    }
}

void BookCache::loadThumbImg(PageNum seqNum, std::function<void(const OpenSessionId &sessionId, const QPixmap &img)> &&cb)
{
    exec([this, seqNum, cb = std::move(cb)]() mutable {
        actor_->loadThumbImg(seqNum, std::move(cb));
    }, Prio::Gui);
}

void BookCache::loadPageImg(PageNum seqNum, std::function<void(const OpenSessionId &sessionId, const QPixmap &img)> &&cb)
{
    exec([this, seqNum, cb = std::move(cb)]() mutable {
        actor_->loadPageImg(seqNum, std::move(cb));
    }, Prio::Gui);
}

void BookCache::Actor::loadThumbImg(PageNum seqNum, std::function<void(const OpenSessionId &sessionId, const QPixmap &img)> &&cb)
{
    Buff data = stmtQueryThumbImg_(seqNum);

    QPixmap img;
    img.loadFromData(reinterpret_cast<const uchar *>(data.data()), static_cast<uint>(data.size()));

    cb(outer_.sessionId_, img);
}

BookCache::Actor::Actor(BookCache &outer)
    : outer_(outer)
    , handle_(*this)
{
    if (*dopLog) {
        gLogger.d << "BookCache::Actor::Actor() begin";
    }

    bindSequencerSignals();

    prepareDb();

    const std::set<u32str> cachedEntries = handleCachedEntries();

    loader_ = std::make_unique<BookLoader>(outer_.archiveFile_, cachedEntries);
    bindLoaderSignals();
    loader_->start();


    props_.setLoadStartTime(LoadClock::now());
}

BookCache::Actor::~Actor()
{
    if (*dopLog) {
        gLogger.d << "BookCache::Actor::~Actor() end";
    }
}

void myapp::BookCache::Actor::loadPageImg(PageNum seqNum, std::function<void(const OpenSessionId &sessionId, const QPixmap &img)> &&cb)
{
    Buff data = stmtQueryRawImg_(seqNum);

    QPixmap img;
    img.loadFromData(reinterpret_cast<const uchar *>(data.data()), static_cast<uint>(data.size()));

    cb(outer_.sessionId_, img);
}

void BookCache::Actor::bindSequencerSignals()
{
    sequencerSigConns_.clear();

    sequencerSigConns_ += sequencer_.sigPageOrdered.connect([this, h = handle_.weak()](sptr<PageInfo> page) {
        h.apply([this, page] {
            outer_.exec([this, page] {
                outer_.sigPageLoaded(outer_.openSessionId(), *page);
            }, Prio::Logic);
        });
    });
}

void BookCache::Actor::bindLoaderSignals()
{
    loaderSigConns_.clear();

    loaderSigConns_ += loader_->sigPageLoaded.connect([this, h = handle_.weak()](sptr<LoadedPage> page) {
        h.apply([this, page] {
            outer_.exec([this, page] {
                onPageLoaded(page);
            }, Prio::Logic);
        });
    });

    loaderSigConns_ += loader_->sigBookLoaded.connect([this, h = handle_.weak()](i32 totalPageCount) {
        h.apply([this, totalPageCount] {
            outer_.exec([this, totalPageCount] {
                onBookLoaded(totalPageCount);
            }, Prio::Logic);
        });
    });
}

void BookCache::Actor::prepareDb()
{
    fs::create_directories(outer_.dbFile_.parent_path());

    if (outer_.shouldForceReload_ == ShouldForceReload::Yes) {
        fs::remove(outer_.dbFile_);
    }

    db_.open(outer_.dbFile_);

    db_.exec(kSqlCreateTables);

    props_.open(db_);

    stmtSavePage_.open(db_);

    stmtWalkPageInfos_.open(db_);

    stmtQueryThumbImg_.open(db_);

    stmtQueryRawImg_.open(db_);
}

void BookCache::Actor::onPageLoaded(sptr<LoadedPage> page)
{
    if (*dopLog) {
        gLogger.d << "onPageLoaded: " << page->name;
    }

    // save data to db
    PageDbData data;
    data.seqNum = page->seqNum;
    data.name = page->name;
    data.rawWidth = page->rawWidth;
    data.rawHeight = page->rawHeight;
    data.rawImg = std::move(page->encodedRawImg);
    data.scaledImg = std::move(page->encodedScaledImg);

    stmtSavePage_(data);


    // feed unordered page to sequencer
    sptr<PageInfo> pageInfo = std::make_shared<PageInfo>();
    pageInfo->seqNum = page->seqNum;
    pageInfo->name = page->name;
    pageInfo->rawWidth = page->rawWidth;
    pageInfo->rawHeight = page->rawHeight;

    sequencer_.feed(pageInfo);
}

void BookCache::Actor::onBookLoaded(i32 totalPageCount)
{
    if (*dopLog) {
        gLogger.d << "onBookLoaded";
    }

    props_.setLoadSucceedTime(LoadClock::now());
    props_.setTotalPageCount(totalPageCount);
}

std::set<u32str> BookCache::Actor::handleCachedEntries()
{
    std::set<u32str> cachedEntries;

    stmtWalkPageInfos_.walk([this, &cachedEntries](sptr<PageInfo> page) {
        cachedEntries.insert(page->name);
        sequencer_.feed(page);
    });

    return cachedEntries;
}

void BookCache::Props::open(sqlite::Database &db)
{
    propRepo_.open(db, "book_props");
}

void BookCache::Props::close()
{
    propRepo_.close();
}

void BookCache::Actor::StmtSavePage::open(sqlite::Database &db)
{
    stmt_.open(db, "insert or replace into pages values (?,?,?,?,?,?);");
}

void BookCache::Actor::StmtSavePage::close()
{
    stmt_.close();
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

void BookCache::Actor::StmtWalkPageInfos::open(sqlite::Database &db)
{
    stmt_.open(db, "select seqNum,name,rawWidth,rawHeight from pages order by seqNum asc;");
}

void BookCache::Actor::StmtWalkPageInfos::close()
{
    stmt_.close();
}

void BookCache::Actor::StmtWalkPageInfos::walk(std::function<void(sptr<PageInfo> page)> &&cb)
{
    stmt_.reset();

    while (stmt_.nextRow()) {
        sptr<PageInfo> info = std::make_shared<PageInfo>();
        stmt_.getValue(0, info->seqNum);
        stmt_.getValue(1, info->name);
        stmt_.getValue(2, info->rawWidth);
        stmt_.getValue(3, info->rawHeight);
        cb(info);
    }
}

void BookCache::Actor::StmtQueryThumbImg::open(sqlite::Database &db)
{
    stmt_.open(db, "select scaledImg from pages where seqNum = ?;");
}

void myapp::BookCache::Actor::StmtQueryThumbImg::close()
{
    stmt_.close();
}

Buff myapp::BookCache::Actor::StmtQueryThumbImg::operator()(PageNum seqNum)
{
    stmt_.reset();

    stmt_.arg(seqNum);

    Buff data;

    if (stmt_.nextRow()) {
        stmt_.getValue(0, data);
    } else {
        if (*dopLog) {
            gLogger.e << "StmtQueryThumbImg execute failed";
        }
    }

    return data;
}

void BookCache::Actor::StmtQueryRawImg::open(sqlite::Database &db)
{
    stmt_.open(db, "select rawImg from pages where seqNum = ?;");
}

void myapp::BookCache::Actor::StmtQueryRawImg::close()
{
    stmt_.close();
}

Buff myapp::BookCache::Actor::StmtQueryRawImg::operator()(PageNum seqNum)
{
    stmt_.reset();

    stmt_.arg(seqNum);

    Buff data;

    if (stmt_.nextRow()) {
        stmt_.getValue(0, data);
    } else {
        if (*dopLog) {
            gLogger.e << "StmtQueryRawImg execute failed";
        }
    }

    return data;
}

}
