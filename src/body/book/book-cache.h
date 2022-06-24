#pragma once

#include "core/fs.h"
#include "core/thread.h"

#include "util/sqlite.h"
#include "util/prop-repo.h"

#include "page-sequencer.h"
#include "book-loader.h"
#include "page-db-data.h"
#include "page-info.h"

#include "open-session-id.h"
#include "book-operation-priority.h"
#include "inner_options.h"


namespace myapp {

class BookCache : public PrioritySingleThreadStrand<BookOperationPriority> {
public:
    BookCache(const OpenSessionId &sessionId, const fs::path &archiveFile, const fs::path &dbFile, ShouldForceReload shouldForceReload);

    ~BookCache();

    const OpenSessionId &openSessionId() {
        return sessionId_;
    }

    using CbPageLoaded = void(const OpenSessionId &sessionId, const PageInfo &page);

    Signal<CbPageLoaded> sigPageLoaded;

    void loadThumbImg(PageNum seqNum, std::function<void(const OpenSessionId &sessionId, const QPixmap &img)> &&cb);

private:

    using Prio = BookOperationPriority;

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

        void loadThumbImg(PageNum seqNum, std::function<void(const OpenSessionId &sessionId, const QPixmap &img)> &&cb);

    private:
        void bindSequencerSignals();

        void bindLoaderSignals();

        void prepareDb();

        void onPageLoaded(sptr<LoadedPage> page);

        void onBookLoaded(i32 totalPageCount);

        std::set<u32str> handleCachedEntries();

        class StmtSavePage {
        public:
            void open(sqlite::Database &db);

            void close();

            void operator()(const PageDbData &page);

        private:
            sqlite::Statement stmt_;
        };

        class StmtWalkPageInfos {
        public:
            void open(sqlite::Database &db);

            void close();

            void walk(std::function<void(sptr<PageInfo> page)> &&cb);

        private:
            sqlite::Statement stmt_;
        };

        class StmtQueryThumbImg {
        public:
            void open(sqlite::Database &db);

            void close();

            Buff operator()(PageNum seqNum);

        private:
            sqlite::Statement stmt_;
        };

    private:
        BookCache &outer_;
        PageSequencer sequencer_;
        uptr<BookLoader> loader_;
        SigConns sequencerSigConns_;
        SigConns loaderSigConns_;
        sqlite::Database db_;
        Props props_;
        StmtSavePage stmtSavePage_;
        StmtWalkPageInfos stmtWalkPageInfos_;
        StmtQueryThumbImg stmtQueryThumbImg_;
        StrongHandle<Actor> handle_;
    };

private:
    const OpenSessionId sessionId_;
    const fs::path archiveFile_;
    const fs::path dbFile_;
    const ShouldForceReload shouldForceReload_;
    uptr<Actor> actor_;
};

}
