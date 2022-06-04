#pragma once

#include "core/fs.h"
#include "core/ustr.h"


namespace myapp {

// 图片位置
// 用于唯一地定位图片
class ImgLoc {
public:
    ImgLoc() {}

    ImgLoc(const fs::path &archiveFilePath, const u8str &entryPath)
        : archiveFilePath(archiveFilePath)
        , entryPath(entryPath) {}

    // 包路径
    // 包可能是压缩包文件、归档文件、PDF，以及由很多图片组成的目录
    // TODO 未来可能改成用 URI 来表示更多种类的数据源
    fs::path archiveFilePath;

    // 图片在包中的路径
    u8str entryPath;
};

}
