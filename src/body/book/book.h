#pragma once

#include "core/fs.h"
#include "core/thread.h"
#include "core/basetype.h"
#include "core/declare_ptr.h"

#include "util/signal.h"
#include "util/sigconns.h"

#include "engine/engine.h"

#include "page-info.h"
#include "page-num.h"

#include "open-session-id.h"
#include "inner_options.h"


class QPixmap;

namespace myapp {

class BookCache;

// 代表抽象的书
// GUI模块和非GUI模块的总界面
// GUI模块不关心下层的诸如缓存、加载等逻辑的细节
class Book : public SingleThreadStrand {
public:
    explicit Book(Engine &engine);

    virtual ~Book();

    void close();

    void open(const fs::path &archiveFile);

    void reload();

    using CbBookOpenStarted = void(const fs::path &archiveFile);

    Signal<CbBookOpenStarted> sigBookOpenStarted;

    using CbPageLoaded = void(const PageInfo &page);

    Signal<CbPageLoaded> sigPageLoaded;

    using CbBookClosed = void(const fs::path &archiveFile);

    Signal<CbBookClosed> sigBookClosed;

    void loadThumbImg(PageNum seqNum, std::function<void(const QPixmap &img)> &&cb);

private:

    class Actor {
    public:
        Actor(Book &outer);

        void close();

        void open(const fs::path &archiveFile);

        void reload();

        void loadThumbImg(PageNum seqNum, std::function<void(const QPixmap &img)> &&cb);

    private:
        void open(const fs::path &archiveFile, ShouldForceReload shouldForceReload);

        void asyncDeleteBookCache();

    private:
        // 不会随着书本的打开关闭而变化的成员
        Book &outer_;
        OpenSessionIdGenerator sessionIdGen_{ 1 };
        

        // 会随着书本的打开关闭而变化的成员
        fs::path archiveFile_;
        const OpenSessionId invalidSessionId_{ -1 };
        OpenSessionId currentSessionId_{ invalidSessionId_ };
        DeclarePtr<BookCache> cache_;
        SigConns sigConns_;


        // 始终保持为最后一个成员
        StrongHandle<Actor> handle_;
    };

private:
    uptr<Actor> actor_;
    Engine &engine_;
};


}
