#pragma once

#include "fs.h"


namespace myapp::wrapper::libarchive {

class ArchiveImpl;

class Archive {
public:
    Archive(const fs::path &file);

    ~Archive();

private:
    ArchiveImpl *impl_{};
};

}
