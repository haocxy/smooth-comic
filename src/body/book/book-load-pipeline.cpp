#include "book-load-pipeline.h"

#include "util/img-util.h"


namespace myapp {

static const char *const kEncodeFormat = "WEBP";

static i32 decideDecoderCount() {
    return 1;
}

static i32 decideScalerCount() {
    return 1;
}

static i32 decideEncoderCount() {
    return 1;
}

BookLoadPipeline::BookLoadPipeline(uptr<PageDataLoader> &&pageDataLoader, Allocator allocator)
    : pageDataLoader_(std::move(pageDataLoader))
    , allocator_(allocator)
{
    sigConnsPageDataLoader_ += pageDataLoader_->sigPageDataLoaded.connect([this](sptr<PageData> data) {
        dataQueue_.push(std::move(*data));
    });

    const i32 decoderCount = decideDecoderCount();
    for (i32 i = 0; i < decoderCount; ++i) {
        decoders_.push_back(std::make_unique<PageDecoder>(stopped_, dataQueue_, rawImgQueue_));
    }

    const i32 scalerCount = decideScalerCount();
    for (i32 i = 0; i < scalerCount; ++i) {
        scalers_.push_back(std::make_unique<PageScaler>(stopped_, rawImgQueue_, scaledImgQueue_));
    }

    const i32 encoderCount = decideEncoderCount();
    for (i32 i = 0; i < encoderCount; ++i) {
        encoders_.push_back(std::make_unique<PageEncoder>(stopped_, scaledImgQueue_, sigPageLoaded, allocator_));
    }
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
    img.loadFromData(reinterpret_cast<const uchar *>(pageData.data.data()), static_cast<int>(pageData.data.size()));

    PageRawImg pageImg;
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
    page->name = std::move(scaledImg.name);
    page->rawWidth = scaledImg.rawImg.width();
    page->rawHeight = scaledImg.rawImg.height();
    page->encodedRawImg = ImgUtil::toSccBuff(scaledImg.rawImg, kEncodeFormat, allocator_);
    page->encodedScaledImg = ImgUtil::toSccBuff(scaledImg.scaledImg, kEncodeFormat, allocator_);

    sigPageLoaded_(page);
}

}
