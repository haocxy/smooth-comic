#pragma once

#include <string>

#include "core/fs.h"


namespace myapp::FsUtil {

// 把路径编码为一个单一的文件名
// 
// 目录本身会占用大量的硬盘容量，所以不能直接把文档路径映射为数据库的路径，
// 而是应该把文档路径编码为一个单一的文件名
std::u32string encodePathToName(const fs::path &path);

}
