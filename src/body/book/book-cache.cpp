#include "book-cache.h"

#include "engine/engine.h"
#include "engine/path-manager.h"

#include "page-loader.h"
#include "thumb-cache.h"
#include "img-cache.h"


namespace myapp {

BookCache::BookCache(Engine &engine, const fs::path &archiveFile)
    : engine_(engine)
    , archiveFile_(archiveFile)
{
    setActorName("BookCache");

    loader_ = new PageLoader(archiveFile_);
    thumbCache_ = new ThumbCache(engine_.pathManager().mkThumbCacheDbFilePath(archiveFile_));
    imgCache_ = new ImgCache(engine_.pathManager().mkImgCacheDbFilePath(archiveFile_));
}

}
