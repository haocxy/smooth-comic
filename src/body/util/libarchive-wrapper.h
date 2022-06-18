#pragma once


#include "core/fs.h"
#include "core/ustr.h"
#include "core/scoped_container.h"


namespace myapp::wrapper::libarchive {

class ArchiveImpl;

class Archive {
public:
    using Allocator = std::pmr::polymorphic_allocator<std::byte>;
    using allocator_type = Allocator;

    Archive(const fs::path &file, Allocator allocator);

    ~Archive();

    bool nextEntry();

    u32str path() const;

    std::string readData();

    scc::buff readContent();

private:
    ArchiveImpl *impl_{};
};

}
