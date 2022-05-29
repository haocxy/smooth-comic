#include "img-archive.h"

#include "util/libarchive-wrapper.h"


namespace myapp {

void ImgArchive::load(const fs::path &archiveFile, OnImgLoad &&cb)
{
    cb_ = std::move(cb);

    wrapper::libarchive::Archive archive(archiveFile);


}

}
