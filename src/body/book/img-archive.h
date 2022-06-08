#pragma once

#include <functional>
#include <QPixmap>

#include "core/fs.h"
#include "core/ustr.h"


namespace myapp {

class ImgArchive {
public:
    using OnImgLoad = std::function<bool(const u32str &path, const QPixmap &img)>;

    void load(const fs::path &archiveFile, OnImgLoad &&cb);
};

}
