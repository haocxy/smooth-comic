#pragma once

namespace myapp::vfs {

class PathImpl {
public:
    virtual ~PathImpl() {}

    virtual bool isFile() const = 0;

    virtual bool isDir() const = 0;
};

}
