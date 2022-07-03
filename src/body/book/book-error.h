#pragma once

namespace myapp {

enum class BookError {

    // 无错误
    // 这个枚举仅用于把特殊值 0 的位置占掉，实际不使用
    NoError = 0,

    // 无效的压缩包
    BadArchive,
};

}
