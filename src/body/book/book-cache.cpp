#include "book-cache.h"

#include "core/logger.h"
#include "engine/engine.h"
#include "engine/path-manager.h"

#include "page-loader.h"
#include "thumb-cache.h"
#include "img-cache.h"


namespace myapp {

using logger::gLogger;

BookCache::BookCache(Engine &engine, const fs::path &archiveFile)
    : engine_(engine)
    , archiveFile_(archiveFile)
    , logger_(*this)
{
    setActorName("BookCache");

    loader_ = new PageLoader(archiveFile_);
    thumbCache_ = new ThumbCache(engine_.pathManager().mkThumbCacheDbFilePath(archiveFile_));
    imgCache_ = new ImgCache(engine_.pathManager().mkImgCacheDbFilePath(archiveFile_));

    logger_.d << "constructed";
}

void BookCache::onMessage(actor::Message &msg)
{
    if (OpenBookMsg *m = msg) {
        logger_.d << "on OpenBookMsg";
        return;
    }

    if (PageLoader::PageLoadedMsg *m = msg) {
        logger_.d << "on PageLoadedMsg";
        return;
    }
}

}
