#pragma once

#include "core/fs.h"
#include "core/thread.h"

#include "page-data-loader.h"


namespace myapp {

class LocalArchivePageDataLoader : public PageDataLoader {
public:
    using Allocator = std::pmr::polymorphic_allocator<std::byte>;

    LocalArchivePageDataLoader(const fs::path &archiveFile, const std::set<u32str> &loadedEntries);

    ~LocalArchivePageDataLoader();

    void start() override;

private:
    void threadBody();

private:
    const fs::path archiveFile_;
    const std::set<u32str> loadedEntries_;
    std::atomic_bool stopped_{ false };
    jthread thread_;
};

}
