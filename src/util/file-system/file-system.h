#pragma once

#include "path.h"


namespace myapp::file_system {

/**
 * 文件系统.
 * 
 * 提供面向对象风格的文件系统，便于管理多种文件系统.
 */
class FileSystem {
public:
    virtual ~FileSystem() {}

    virtual Path root() = 0;
};

}
