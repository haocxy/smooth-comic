#pragma once

#include <functional>
#include <QImage>

#include "util/fs.h"

#include "image.h"


namespace myapp {

class ImgArchive {
public:
    using OnImgLoad = std::function<void(int32_t index, const QString &path, const QImage &img)>;

    void load(const fs::path &archiveFile, OnImgLoad &&cb);

private:
    OnImgLoad cb_;
};

}
