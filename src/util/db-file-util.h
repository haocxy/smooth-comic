#pragma once

#include "core/fs.h"


namespace myapp::DbFileUtil
{

fs::path filePathToDbPath(const fs::path &dbDir, const fs::path &filePath);

void removeUselessDbFiles(const fs::path &dbDir);

}
