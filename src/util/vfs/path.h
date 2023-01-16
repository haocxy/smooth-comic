#pragma once

#include <assert.h>

#include "core/basetype.h"

#include "path-impl.h"


namespace myapp::vfs {

class PathImpl;

/** 虚拟路径 */
class Path {
public:
    Path(uptr<PathImpl>&& pathImpl)
        : impl_(std::move(pathImpl)) {
        assert(impl_);
    }

    /**
     * \brief 是否是文件
     * 
     * \brief 注意！可能既不是文件也不是目录
     * 
     * \return 是文件返回 true，不是文件返回 false
     */
    bool isFile() const {
        return impl_->isFile();
    }

    /**
     * \brief 是否是目录
     * 
     * \brief 注意！可能既不是文件也不是目录
     * 
     * \return 是目录返回 true，不是目录返回 false
     */
    bool isDir() const {
        return impl_->isDir();
    }

private:
    uptr<PathImpl> impl_;
};

}
