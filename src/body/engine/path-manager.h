#pragma once

#include "core/fs.h"


namespace myapp {


// 统一管理程序中的各种路径
class PathManager {
public:
    PathManager();

    fs::path mkBookCacheDbFilePath(const fs::path &archiveFile) const;

    fs::path packedStyleDir() const;

private:
    fs::path cacheDir_;
};

}
