#include "book.h"

#include <cstring>
#include <condition_variable>

#include "core/fs.h"
#include "core/logger.h"

#include "engine/path-manager.h"
#include "engine/async-deleter.h"


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

void Book::open(const fs::path &archiveFile)
{
    exec([this, archiveFile] {
        actor_->open(archiveFile);
    });
}

void Book::Actor::asyncDeleteBookCache()
{
    cache_ = nullptr;
}

Book::Actor::Actor(Book &outer)
    : outer_(outer)
    , handle_(*this)
{
}

void Book::Actor::open(const fs::path &archiveFile)
{
    if (!fs::is_regular_file(archiveFile)) {
        return;
    }

    archiveFile_ = archiveFile;

    asyncDeleteBookCache();

    cache_ = std::make_unique<BookCache>(archiveFile, outer_.engine_.pathManager().mkBookCacheDbFilePath(archiveFile));

    sigConns_.clear();

    sigConns_ += cache_->sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page){
        h.apply([this, &page] {
            outer_.exec([this, page] {
                outer_.sigPageLoaded(page);
            });
        });
    });

    outer_.sigBookOpenStarted(archiveFile);
}

}
