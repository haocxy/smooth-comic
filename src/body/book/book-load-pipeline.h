#pragma once

#include <QImage>

#include "core/fs.h"
#include "core/thread.h"

#include "util/signal.h"
#include "util/sigconns.h"

#include "page-data-loader.h"
#include "loaded-page.h"


namespace myapp {

// 书籍加载流水线
//
// 控制书籍加载的过程，一次性使用，加载完成后即可删除流水线对象
class BookLoadPipeline {
public:
    using Allocator = std::pmr::polymorphic_allocator<std::byte>;

    BookLoadPipeline(uptr<PageDataLoader> &&pageDataLoader, Allocator allocator);

    ~BookLoadPipeline();

    void start() {
        pageDataLoader_->start();
    }

    using CbPageLoaded = void(sptr<LoadedPage> loadedPage);

    Signal<CbPageLoaded> sigPageLoaded;

    using CbPageCountDetected = void(i32 totalPageCount);

    Signal<CbPageCountDetected> sigPageCountDetected;

private:


    class PageRawImg {
    public:
        PageRawImg() {}

        PageRawImg(PageRawImg &&other) noexcept
            : name(std::move(other.name)), img(std::move(other.img)) {}

        u32str name;
        QImage img;
    };

    class PageScaledImg {
    public:
        PageScaledImg() {}

        u32str name;
        QImage rawImg;
        QImage scaledImg;
    };

    // 页面解码器
    // 用于把从 页面数据加载器 得到的原始图片数据解码为统一的内部格式
    class PageDecoder {
    public:
        PageDecoder(std::atomic_bool &stopped, BlockQueue<PageData> &dataQueue, BlockQueue<PageRawImg> &rawImgQueue)
            : stopped_(stopped), dataQueue_(dataQueue), rawImgQueue_(rawImgQueue), thread_([this] { loop(); }) {}

    private:
        void loop();

        void handle(PageData &&pageData);

    private:
        std::atomic_bool &stopped_;
        BlockQueue<PageData> &dataQueue_;
        BlockQueue<PageRawImg> &rawImgQueue_;
        std::jthread thread_;
    };

    // 页面缩放器
    // 用于为从 页面解码器 得到的统一的原始尺寸的图片创建缩小的图片作为缩略图
    //
    // 问：为什么要分别存储原始图片和缩略图？
    // 答：原始图片和缩略图的使用方式有很大差异：
    //     原始图片通常顺序使用，且一次只显示相连的有限几张（取决于阅读方式），有较强的局部性；
    //     而缩略图则通常会被很快地切换，或者在一个显示很多缩略图的视图中使用，有较强的随机性。
    class PageScaler {
    public:
        PageScaler(std::atomic_bool &stopped, BlockQueue<PageRawImg> &rawImgQueue, BlockQueue<PageScaledImg> &scaledImgQueue)
            : stopped_(stopped), rawImgQueue_(rawImgQueue), scaledImgQueue_(scaledImgQueue), thread_([this] { loop(); }) {}

    private:
        void loop();

        void handle(PageRawImg &&rawImg);

    private:
        std::atomic_bool &stopped_;
        BlockQueue<PageRawImg> &rawImgQueue_;
        BlockQueue<PageScaledImg> &scaledImgQueue_;
        std::jthread thread_;
    };

    // 页面编码器
    // 用于把从 页面缩放器 得到的原始图片和缩放图片编码为将要存储到本地数据库中的形式
    class PageEncoder {
    public:
        PageEncoder(std::atomic_bool &stopped, BlockQueue<PageScaledImg> &scaledImgQueue, Signal<CbPageLoaded> &sigPageLoaded, Allocator &allocator)
            : stopped_(stopped), scaledImgQueue_(scaledImgQueue), sigPageLoaded_(sigPageLoaded), allocator_(allocator), thread_([this] { loop(); }) {}

    private:
        void loop();

        void handle(PageScaledImg &&scaledImg);

    private:
        std::atomic_bool &stopped_;
        BlockQueue<PageScaledImg> &scaledImgQueue_;
        Signal<CbPageLoaded> &sigPageLoaded_;
        Allocator &allocator_;
        std::jthread thread_;
    };

private:
    uptr<PageDataLoader> pageDataLoader_;
    Allocator allocator_;
    SigConns sigConnsPageDataLoader_;


    BlockQueue<PageData> dataQueue_;
    BlockQueue<PageRawImg> rawImgQueue_;
    BlockQueue<PageScaledImg> scaledImgQueue_;


    std::atomic_bool stopped_{ false };
    std::vector<uptr<PageDecoder>> decoders_;
    std::vector<uptr<PageScaler>> scalers_;
    std::vector<uptr<PageEncoder>> encoders_;
};

}
