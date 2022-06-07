#include "path-manager.h"

#include "util/fs-util.h"


namespace myapp {

static const std::u32string kDbFileExt{ U".smooth-comic-db" };

PathManager::PathManager()
{
    cacheDir_ = "D:/smooth-comic-cache";
}

fs::path PathManager::mkThumbCacheDbFilePath(const fs::path &archiveFile) const
{
    return cacheDir_ / (FsUtil::encodePathToName(archiveFile) + U".thumb-cache" + kDbFileExt);
}

fs::path PathManager::mkImgCacheDbFilePath(const fs::path &archiveFile) const
{
    return cacheDir_ / (FsUtil::encodePathToName(archiveFile) + U".img-cache" + kDbFileExt);
}

}
