#pragma once

#include <functional>
#include <QPixmap>

#include "core/fs.h"

#include "image.h"


namespace myapp {

class ImgArchive {
public:
    using OnImgLoad = std::function<bool(const QString &path, const QPixmap &img)>;

    void load(const fs::path &archiveFile, OnImgLoad &&cb);
};

}
