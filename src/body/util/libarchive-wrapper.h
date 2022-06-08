#pragma once

#include "core/fs.h"
#include "core/ustr.h"


namespace myapp::wrapper::libarchive {

class ArchiveImpl;

class Archive {
public:
    Archive(const fs::path &file);

    ~Archive();

    bool nextEntry();

    u8str path() const;

    std::string readData();

private:
    ArchiveImpl *impl_{};
};

}
