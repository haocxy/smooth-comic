#include "path-manager.h"

#include "core/system.h"
#include "util/fs-util.h"


#include "build-info.h"


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

fs::path PathManager::packedStyleDir() const
{
    const fs::path packPath = SystemUtil::exePath().parent_path() / "-styles";
    if (fs::exists(packPath)) {
        return packPath;
    }

    const std::optional<fs::path> &slnDir = BuildInfo::solutionDir;
    if (slnDir) {
        const fs::path slnPath = (*slnDir) / "-styles";
        if (fs::exists(slnPath)) {
            return slnPath;
        }
    }

    throw std::logic_error("PathManager::packedStyleDir() cannot find style dir");
}

}
