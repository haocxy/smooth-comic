#pragma once

#include "core/fs.h"


namespace myapp {


// 统一管理程序中的各种路径
class PathManager {
public:
    PathManager();

    fs::path mkBookCacheDbFilePath(const fs::path &archiveFile) const;

    fs::path mkThumbCacheDbFilePath(const fs::path &archiveFile) const;

    fs::path mkImgCacheDbFilePath(const fs::path &archiveFile) const;

private:
    fs::path cacheDir_;
};

}
