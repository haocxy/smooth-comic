#pragma once

#include "util/file-system/file-system.h"


namespace myapp::file_system::impl::win32 {

class LocalDiskFileSystem : public FileSystem {
public:
    virtual ~LocalDiskFileSystem();

    virtual Path root() override;

private:

};

}
