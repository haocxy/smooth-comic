#include "img-archive.h"

#include <iostream>
#include <QDebug>

#include "util/libarchive-wrapper.h"


namespace myapp {

void ImgArchive::load(const fs::path &archiveFile, OnImgLoad &&cb)
{
    wrapper::libarchive::Archive archive(archiveFile);

    while (archive.nextEntry()) {
        std::u8string path = archive.path();
        QString qPath = QString::fromUtf8(path.c_str());
        qInfo() << "path:" << qPath;
        std::string data = archive.readData();
        qInfo() << "data bytes:" << data.size() << "B";
        QImage img;
        img.loadFromData(QByteArrayView(data.data(), data.size()));
        qInfo() << "img size: (" << img.width() << ", " << img.height() << ")";

        const bool goon = cb(qPath, img);
        if (!goon) {
            break;
        }
    }
}

}
