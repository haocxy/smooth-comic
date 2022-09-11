#include "path-manager.h"

#include <QDebug>
#include <QSysInfo>
#include <QStandardPaths>

#include "core/system.h"
#include "core/logger.h"
#include "util/fs-util.h"

#include "build-info.h"


namespace myapp {

using logger::gLogger;

static const std::u32string kCacheFileExt{ U".sMo-OtH-_C-oMi-C--CaC_hE" };

namespace DirName {

static const std::u32string Styles = U"styles";
static const std::u32string Book = U"book";

}

static fs::path selectDefaultBaseCacheDir()
{
    if (SystemUtil::platformType == PlatformType::Android) {
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation).toStdU32String();
    }

    if (SystemUtil::platformType != PlatformType::Windows) {
        return SystemUtil::userHome();
    }

    return "C:/";
}

static fs::path selectDefaultCacheDir()
{
    const fs::path base = selectDefaultBaseCacheDir();
    qDebug() << "selectDefaultBaseCacheDir: " << QString::fromStdU32String(base.generic_u32string());
    return base / ".smooth-comic-cache";
}

PathManager::PathManager()
{
    cacheDir_ = selectDefaultCacheDir();
}

fs::path PathManager::mkBookCacheDbFilePath(const fs::path &archiveFile) const
{
    return cacheDir_ / DirName::Book / (FsUtil::encodePathToName(archiveFile) + kCacheFileExt);
}

fs::path PathManager::packedStyleDir() const
{
    const std::u32string styleDirName = DirName::Styles;

    const fs::path exeDir = SystemUtil::exePath().parent_path();

    const fs::path packPath = exeDir / styleDirName;
    if (fs::exists(packPath)) {
        return packPath;
    }

    const std::optional<fs::path> &slnDir = BuildInfo::solutionDir;
    if (slnDir) {
        const fs::path slnPath = (*slnDir) / styleDirName;
        if (fs::exists(slnPath)) {
            return slnPath;
        }
    }

    for (fs::path dir = exeDir.parent_path();;) {
        const fs::path styleDirPath = dir / styleDirName;
        if (fs::exists(styleDirPath)) {
            return styleDirPath;
        }
        const fs::path parentDir = dir.parent_path();
        if (parentDir != dir) {
            dir = parentDir;
        } else {
            break;
        }
    }

    throw std::logic_error("PathManager::packedStyleDir() cannot find style dir");
}

fs::path PathManager::bookCacheDir() const
{
    return cacheDir_ / DirName::Book;
}

bool PathManager::isCacheFile(const fs::path &path) const
{
    return path.generic_u32string().ends_with(kCacheFileExt);
}

}
