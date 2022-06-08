#pragma once

#include "core/fs.h"
#include "core/logger.h"
#include "core/declare_ptr.h"
#include "util/actor.h"


namespace myapp {

class Engine;
class PageLoader;
class ThumbCache;
class ImgCache;


// 整个缓存系统的对外接口
// 其它缓存模块（例如缩略图缓存和图片缓存）都隐藏在这个模块的后面，上层UI逻辑只直接使用这个模块
class BookCache : public actor::ThreadedActor {
public:

    class OpenBookMsg : public actor::Message {
    public:
        OpenBookMsg() {}
    };

    BookCache(Engine &engine, const fs::path &archiveFile);

protected:
    void onMessage(actor::Message &msg) override;

private:
    class Logger : public logger::Logger {
    public:
        Logger(BookCache &self) : self_(self) {
            setLevel(logger::Level::All);
        }

        void beforeWriteContent(logger::LogStream &stream) {
            stream << "BookCache(" << self_.archiveFile_ << ") ";
        }

    private:
        BookCache &self_;
    };

private:
    Engine &engine_;
    const fs::path archiveFile_;
    Logger logger_;
    DeclarePtr<PageLoader> loader_;
    DeclarePtr<ThumbCache> thumbCache_;
    DeclarePtr<ImgCache> imgCache_;
};

}
