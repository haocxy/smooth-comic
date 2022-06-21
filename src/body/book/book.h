#pragma once

#include "core/fs.h"
#include "core/basetype.h"

#include "util/sigconns.h"

#include "engine/engine.h"

#include "page-num.h"

#include "book-cache.h"


namespace myapp {

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

private:

    class Actor {
    public:
        Actor(Book &outer);

        void open(const fs::path &archiveFile);

    private:
        void asyncDeleteBookCache();

    private:
        Book &outer_;
        
        fs::path archiveFile_;
        uptr<BookCache> cache_;

        SigConns sigConns_;

        StrongHandle<Actor> handle_;
    };

private:
    uptr<Actor> actor_;
    Engine &engine_;
};


}
