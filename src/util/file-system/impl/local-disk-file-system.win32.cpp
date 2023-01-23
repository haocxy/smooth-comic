#include "local-disk-file-system.win32.h"


namespace myapp::file_system::impl::win32 {

LocalDiskFileSystem::~LocalDiskFileSystem() {
}

Path LocalDiskFileSystem::root() {
    return Path();
}

}

