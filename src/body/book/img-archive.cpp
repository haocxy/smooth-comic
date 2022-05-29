#include "img-archive.h"


namespace myapp {

void ImgArchive::load(const fs::path &archiveFile, OnImgLoad &&cb)
{
    cb_ = std::move(cb);


}

}
