#pragma once

#include <functional>

#include "core/basetype.h"
#include "core/thread.h"

#include "util/signal.hpp"

#include "page-data.h"
#include "book-error.h"


namespace myapp {

// 页面数据加载器
// 
// 功能：从某个地方（例如本地压缩文件、网盘压缩文件、或者某个提供线上漫画阅读的网络服务）加载单页漫画的数据，
// 数据为使用某种编码方式编码后的图片数据
//
// 注意：这个模块只需要保证每次返回的数据是一个完整的不多不少的图片的数据即可，不需要关心图片类型
// （因为主流图片格式的编码类型信息会包含在数据中）
class PageDataLoader {
public:
    virtual ~PageDataLoader() {}

    virtual void start() = 0;

    using CbPageDataLoaded = void(sptr<PageData> data);

    Signal<CbPageDataLoaded> sigPageDataLoaded;

    using CbPageCountDetected = void(i32 pageCount);

    // 当这本书中的总页数明确可知时回调
    // 外部逻辑会利用这一信息检查加载流程是否完全结束
    Signal<CbPageCountDetected> sigPageCountDetected;

    using CbFailed = void(BookError err);

    Signal<CbFailed> sigFailed;
};

}
