#include "img-archive.h"

#include <iostream>

#include "util/libarchive-wrapper.h"


namespace myapp {

void ImgArchive::load(const fs::path &archiveFile, OnImgLoad &&cb)
{
    wrapper::libarchive::Archive archive(archiveFile);

    while (archive.nextEntry()) {
        u8str entryPath = archive.path();
        std::string data = archive.readData();
        QPixmap img;
        img.loadFromData(QByteArray(data.data(), data.size()));

        const bool goon = cb(entryPath, img);
        if (!goon) {
            break;
        }
    }
}

}
