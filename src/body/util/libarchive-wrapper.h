#pragma once

#include "fs.h"


namespace myapp::wrapper::libarchive {

class ArchiveImpl;

class Archive {
public:
    Archive(const fs::path &file);

    ~Archive();

    bool nextEntry();

private:
    ArchiveImpl *impl_{};
};

}
