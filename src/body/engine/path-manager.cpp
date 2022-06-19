#include "path-manager.h"

#include "util/fs-util.h"


namespace myapp {

static const std::u32string kDbFileExt{ U"" };

PathManager::PathManager()
{
    cacheDir_ = "D:/smooth-comic-cache";
}

fs::path PathManager::mkBookCacheDbFilePath(const fs::path &archiveFile) const
{
    return cacheDir_ / "book" / (FsUtil::encodePathToName(archiveFile) + U".db" + kDbFileExt);
}

}
