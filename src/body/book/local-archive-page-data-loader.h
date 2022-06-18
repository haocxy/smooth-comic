#pragma once

#include "core/fs.h"
#include "core/thread.h"

#include "page-data-loader.h"


namespace myapp {

class LocalArchivePageDataLoader : public PageDataLoader {
public:
    using Allocator = std::pmr::polymorphic_allocator<std::byte>;

    LocalArchivePageDataLoader(const fs::path &archiveFile, Allocator allocator);

    void start() override;

private:
    void threadBody();

private:
    const fs::path archiveFile_;
    Allocator allocator_;
    std::jthread thread_;
};

}
