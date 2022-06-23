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

    void open(const fs::path &archiveFile);

    using CbBookOpenStarted = void(const fs::path &archiveFile);

    Signal<CbBookOpenStarted> sigBookOpenStarted;

    using CbPageLoaded = void(const PageInfo &page);

    Signal<CbPageLoaded> sigPageLoaded;

    void loadThumbImg(PageNum seqNum, std::function<void(const QPixmap &img)> &&cb);

private:

    class Actor {
    public:
        Actor(Book &outer);

        void open(const fs::path &archiveFile);

        void loadThumbImg(PageNum seqNum, std::function<void(const QPixmap &img)> &&cb);

    private:
        void asyncDeleteBookCache();

    private:
        Book &outer_;
        OpenSessionIdGenerator sessionIdGen_{ 1 };
        
        fs::path archiveFile_;
        OpenSessionId currentSessionId_{ -1 };
        DeclarePtr<BookCache> cache_;

        SigConns sigConns_;

        StrongHandle<Actor> handle_;
    };

private:
    uptr<Actor> actor_;
    Engine &engine_;
};


}
