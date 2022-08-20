#include "book-load-pipeline.h"

#include <QDebug>

#include "util/img-util.h"
#include "core/debug-option.h"


namespace myapp {

static const DebugOption<i64> dopDelayLoadMs("delay.load.ms", 0,
    "Make loader sleep some ms on each entry.");

static const char *const kEncodeFormat = "WEBP";

static i32 decideDecoderCount() {
    return 4;
}

static i32 decideScalerCount() {
    return 4;
}

static i32 decideEncoderCount() {
    return 4;
}

BookLoadPipeline::BookLoadPipeline(uptr<PageDataLoader> &&pageDataLoader)
    : handle_(*this)
    , pageDataLoader_(std::move(pageDataLoader))
    , decoderCount_(decideDecoderCount())
    , scalerCount_(decideScalerCount())
    , encoderCount_(decideEncoderCount())
    , dataQueue_(decoderCount_ * 2)
    , rawImgQueue_(scalerCount_ * 2)
    , scaledImgQueue_(encoderCount_ * 2)
{
    sigConnsPageDataLoader_ += pageDataLoader_->sigPageDataLoaded.connect([this, h = handle_.weak()](sptr<PageData> data) {
        h.apply([this, &data] {
            if (*dopDelayLoadMs > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(*dopDelayLoadMs));
            }
            dataQueue_.push(std::move(*data));
        });
    });

    sigConnsPageDataLoader_ += pageDataLoader_->sigPageCountDetected.connect([this, h = handle_.weak()](i32 totalPageCount) {
        h.apply([this, totalPageCount] {
            sigPageCountDetected(totalPageCount);
        });
    });

    sigConnsPageDataLoader_ += pageDataLoader_->sigFailed.connect([this, h = handle_.weak()](BookError err) {
        h.apply([this, &err] {
            sigFailed(err);
        });
    });

    for (i32 i = 0; i < decoderCount_; ++i) {
        decoders_.push_back(std::make_unique<PageDecoder>(stopped_, dataQueue_, rawImgQueue_));
    }

    for (i32 i = 0; i < scalerCount_; ++i) {
        scalers_.push_back(std::make_unique<PageScaler>(stopped_, rawImgQueue_, scaledImgQueue_));
    }

    for (i32 i = 0; i < encoderCount_; ++i) {
        encoders_.push_back(std::make_unique<PageEncoder>(stopped_, scaledImgQueue_, sigPageLoaded));
    }
}

BookLoadPipeline::~BookLoadPipeline()
{
    stopped_ = true;

    dataQueue_.stop();
    rawImgQueue_.stop();
    scaledImgQueue_.stop();
}

void BookLoadPipeline::PageDecoder::loop()
{
    ThreadUtil::setNameForCurrentThread("BookLoadPipeline.PageDecoder");

    while (!stopped_) {
        std::optional<PageData> optData = dataQueue_.pop();
        if (optData) {
            handle(std::move(*optData));
        }
    }
}

void BookLoadPipeline::PageDecoder::handle(PageData &&pageData)
{
    QImage img;
    img.loadFromData(
        reinterpret_cast<const uchar *>(pageData.data.data()),
        static_cast<int>(pageData.data.size())
    );

    qDebug() << "qmllog: handle pageData: " << QString::fromStdU32String(pageData.name);

    if (img.isNull()) {
        qDebug() << "qmllog: cannot loadFromData: " << QString::fromStdU32String(pageData.name);
    }

    PageRawImg pageImg;
    pageImg.seqNum = pageData.seqNum;
    pageImg.name = std::move(pageData.name);
    pageImg.img = std::move(img);

    rawImgQueue_.push(std::move(pageImg));
}

void BookLoadPipeline::PageScaler::loop()
{
    ThreadUtil::setNameForCurrentThread("BookLoadPipeline.PageScaler");

    while (!stopped_) {
        std::optional<PageRawImg> optRawImg = rawImgQueue_.pop();
        if (optRawImg) {
            handle(std::move(*optRawImg));
        }
    }
}

void BookLoadPipeline::PageScaler::handle(PageRawImg &&rawImg)
{
    PageScaledImg page;
    page.seqNum = rawImg.seqNum;
    page.name = std::move(rawImg.name);
    page.rawImg = std::move(rawImg.img);
    page.scaledImg = page.rawImg.scaledToWidth(400);

    scaledImgQueue_.push(std::move(page));
}

void BookLoadPipeline::PageEncoder::loop()
{
    ThreadUtil::setNameForCurrentThread("BookLoadPipeline.PageEncoder");

    while (!stopped_) {
        std::optional<PageScaledImg> optScaledImg = scaledImgQueue_.pop();
        if (optScaledImg) {
            handle(std::move(*optScaledImg));
        }
    }
}

void BookLoadPipeline::PageEncoder::handle(PageScaledImg &&scaledImg)
{
    sptr<LoadedPage> page = std::make_shared<LoadedPage>();
    page->seqNum = scaledImg.seqNum;
    page->name = std::move(scaledImg.name);
    page->rawWidth = scaledImg.rawImg.width();
    page->rawHeight = scaledImg.rawImg.height();
    page->encodedRawImg = ImgUtil::toBuff(scaledImg.rawImg, kEncodeFormat);
    page->encodedScaledImg = ImgUtil::toBuff(scaledImg.scaledImg, kEncodeFormat);
    page->rawImg = std::move(scaledImg.rawImg);
    page->scaledImg = std::move(scaledImg.scaledImg);

    sigPageLoaded_(page);
}

}
