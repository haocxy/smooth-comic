#pragma once


#include "core/fs.h"
#include "core/ustr.h"
#include "core/basetype.h"


namespace myapp::wrapper::libarchive {

class ArchiveImpl;

class Archive {
public:
    Archive(const fs::path &file);

    ~Archive();

    bool nextEntry();

    u32str path() const;

    bool isRegularFile() const;

    bool isDir() const;

    Buff readContent();

    void skipContent();

private:
    ArchiveImpl *impl_{};
};

}
