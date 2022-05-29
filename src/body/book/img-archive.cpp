#include "img-archive.h"

#include <iostream>
#include <QDebug>

#include "util/libarchive-wrapper.h"


namespace myapp {

void ImgArchive::load(const fs::path &archiveFile, OnImgLoad &&cb)
{
    cb_ = std::move(cb);

    wrapper::libarchive::Archive archive(archiveFile);

    while (archive.nextEntry()) {
        std::u8string path = archive.path();
        QString qs = QString::fromUtf8(path.c_str());
        qInfo() << "path:" << qs;
        std::string data = archive.readData();
        qInfo() << "data bytes:" << data.size() << "B";
        QImage img;
        img.loadFromData(QByteArrayView(data.data(), data.size()));
        qInfo() << "img size: (" << img.width() << ", " << img.height() << ")";
    }
}

}
