#pragma once

#include "core/fs.h"
#include "core/thread.h"

#include "book-loader.h"


namespace myapp {

class BookCache : public SingleThreadStrand {
public:
    BookCache(const fs::path &archiveFile, const fs::path &dbFile);

    ~BookCache();

private:
    class Actor {
    public:
        Actor(BookCache &self);

    private:
        void prepareDb();

    private:
        BookCache &self_;
        uptr<BookLoader> loader_;
        SigConns loaderSigConns_;
    };

private:
    const fs::path archiveFile_;
    const fs::path dbFile_;
    uptr<Actor> actor_;
};

}
