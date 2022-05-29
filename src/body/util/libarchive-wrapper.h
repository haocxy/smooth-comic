#pragma once

#include "fs.h"


namespace myapp::wrapper::libarchive {

class ArchiveImpl;

class Archive {
public:
    Archive(const fs::path &file);

    ~Archive();

    bool nextEntry();

    std::u8string path() const;

    std::string readData();

private:
    ArchiveImpl *impl_{};
};

}
