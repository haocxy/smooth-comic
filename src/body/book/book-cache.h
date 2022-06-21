#pragma once

#include "core/fs.h"
#include "core/thread.h"

#include "util/sqlite.h"
#include "util/prop-repo.h"

#include "book-loader.h"
#include "page-db-data.h"
#include "page-info.h"


namespace myapp {

class BookCache : public SingleThreadStrand {
public:
    BookCache(const fs::path &archiveFile, const fs::path &dbFile);

    ~BookCache();

    using CbPageLoaded = void(const PageInfo &page);

    Signal<CbPageLoaded> sigPageLoaded;

private:

    using LoadClock = std::chrono::system_clock;
    using LoadTime = std::chrono::time_point<LoadClock>;

    class Props {
    public:
        void open(sqlite::Database &db);

        void close();

        void setLoadStartTime(const LoadTime &time) {
            propRepo_.set(keyLoadStartTime, time);
        }

        opt<LoadTime> loadStartTime() const {
            return propRepo_.get(keyLoadStartTime);
        }

        void setLoadSucceedTime(const LoadTime &time) {
            propRepo_.set(keyLoadSucceedTime, time);
        }

        opt<LoadTime> loadSucceedTime() const {
            return propRepo_.get(keyLoadSucceedTime);
        }

        void setTotalPageCount(i32 count) {
            propRepo_.set(keyTotalPageCount, count);
        }

        opt<i32> totalPageCount() const {
            return propRepo_.get(keyTotalPageCount);
        }

    private:
        PropRepo propRepo_;
        const u8view keyLoadStartTime{ u8"load-start-time" };
        const u8view keyLoadSucceedTime{ u8"load-succeed-time" };
        const u8view keyTotalPageCount{ u8"total-page-count" };
    };

    class Actor {
    public:
        Actor(BookCache &outer);

        ~Actor();

    private:
        void prepareDb();

        void onPageLoaded(sptr<LoadedPage> page);

        void onBookLoaded(i32 totalPageCount);

        class StmtSavePage {
        public:
            void open(sqlite::Database &db);

            void close();

            void operator()(const PageDbData &page);

        private:
            sqlite::Statement stmt_;
        };

    private:
        BookCache &outer_;
        uptr<BookLoader> loader_;
        SigConns loaderSigConns_;
        sqlite::Database db_;
        Props props_;
        StmtSavePage stmtSavePage_;
        StrongHandle<Actor> handle_;
    };

private:
    const fs::path archiveFile_;
    const fs::path dbFile_;
    uptr<Actor> actor_;
};

}
