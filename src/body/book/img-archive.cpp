#include "img-archive.h"

#include <iostream>

#include "util/libarchive-wrapper.h"


namespace myapp {

void ImgArchive::load(const fs::path &archiveFile, OnImgLoad &&cb)
{
    wrapper::libarchive::Archive archive(archiveFile);

    while (archive.nextEntry()) {
        std::u8string path = archive.path();
        QString qPath = QString::fromUtf8(path.c_str());
        std::string data = archive.readData();
        QPixmap img;
        img.loadFromData(QByteArray(data.data(), data.size()));

        const bool goon = cb(qPath, img);
        if (!goon) {
            break;
        }
    }
}

}
