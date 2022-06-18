#pragma once

#include "core/fs.h"
#include "core/thread.h"

#include "book-loader.h"


namespace myapp {

class BookCache : public SingleThreadStrand {
public:
    BookCache(const fs::path &archiveFile);

    void load();

private:
    const fs::path archiveFile_;
    uptr<BookLoader> loader_;
    SigConns loaderSigConns_;
};

}
