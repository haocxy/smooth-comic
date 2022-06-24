#include "book.h"

#include <cstring>
#include <condition_variable>

#include <QPixmap>

#include "core/fs.h"
#include "core/logger.h"

#include "engine/path-manager.h"
#include "engine/async-deleter.h"

#include "book-cache.h"


namespace myapp {

using logger::gLogger;

Book::Book(Engine &engine)
    : SingleThreadStrand("Book")
    , engine_(engine)
{
    exec([this] {
        actor_ = std::make_unique<Actor>(*this);
    });
}

Book::~Book()
{
    std::mutex mtxStopped;
    std::condition_variable cvStopped;

    std::unique_lock<std::mutex> lockStopped(mtxStopped);

    exec([this, &cvStopped] {
        actor_ = nullptr;
        stopEventQueue();
        cvStopped.notify_all();
    });

    cvStopped.wait(lockStopped);
}

void Book::close()
{
    exec([this] {
        actor_->close();
    });
}

void Book::open(const fs::path &archiveFile)
{
    exec([this, archiveFile] {
        actor_->open(archiveFile);
    });
}

void Book::reload()
{
    exec([this] {
        actor_->reload();
    });
}

void Book::loadThumbImg(PageNum seqNum, std::function<void(const QPixmap &img)> &&cb)
{
    exec([this, seqNum, cb = std::move(cb)] () mutable {
        actor_->loadThumbImg(seqNum, std::move(cb));
    });
}

void Book::Actor::asyncDeleteBookCache()
{
    cache_ = nullptr;
}

void Book::Actor::loadThumbImg(PageNum seqNum, std::function<void(const QPixmap &img)> &&cb)
{
    cache_->loadThumbImg(seqNum, [this, h = handle_.weak(), cb = std::move(cb)](const OpenSessionId &sessionId, const QPixmap &img) mutable {
        h.apply([this, &sessionId, &img, &cb]{
            outer_.exec([this, sessionId, img = std::move(img), cb = std::move(cb)] {
                if (sessionId == currentSessionId_) {
                    cb(img);
                }
            });
        });
    });
}

Book::Actor::Actor(Book &outer)
    : outer_(outer)
    , handle_(*this)
{
}

void myapp::Book::Actor::close()
{
    const fs::path oldPath = std::move(archiveFile_);

    currentSessionId_ = invalidSessionId_;
    sigConns_.clear();
    cache_ = nullptr;
    archiveFile_.clear();

    outer_.sigBookClosed(oldPath);
}

void myapp::Book::Actor::open(const fs::path &archiveFile)
{
    close();
    open(archiveFile, ShouldForceReload::No);
}

void myapp::Book::Actor::reload()
{
    const fs::path archiveFile = std::move(archiveFile_);
    close();
    open(archiveFile, ShouldForceReload::Yes);
}

void Book::Actor::open(const fs::path &archiveFile, ShouldForceReload shouldForceReload)
{
    if (!fs::is_regular_file(archiveFile)) {
        return;
    }

    archiveFile_ = archiveFile;

    asyncDeleteBookCache();

    currentSessionId_ = sessionIdGen_.next();

    cache_ = new BookCache(
        currentSessionId_,
        archiveFile,
        outer_.engine_.pathManager().mkBookCacheDbFilePath(archiveFile),
        shouldForceReload
    );

    sigConns_.clear();

    sigConns_ += cache_->sigPageLoaded.connect([this, h = handle_.weak()](const OpenSessionId &sessionId, const PageInfo &page){
        h.apply([this, sessionId, &page] {
            outer_.exec([this, sessionId, page] {
                if (sessionId == currentSessionId_) {
                    outer_.sigPageLoaded(page);
                }
            });
        });
    });

    outer_.sigBookOpenStarted(archiveFile);
}

}
