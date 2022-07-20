#pragma once

#include "core/basetype.h"

#include "libarchive-wrapper.h"

namespace myapp {

class RandomAccessArchive {
public:
    class Entry {
    public:
        u32str name;
        Buff data;
    };

    RandomAccessArchive() {}

    RandomAccessArchive(const fs::path &file) {
        open(file);
    }

    ~RandomAccessArchive() {
        close();
    }

    void open(const fs::path &file);

    void close();

    i32 entryCount() const {
        return entryCount_;
    }

    Entry entryAt(i32 index);

private:
    void seekCurEntryTo(i32 index);

    void checkIndex(i32 index) const {
        if (0 <= index && index < entryCount_) {
            return;
        }

        throw std::out_of_range("RandomAccessArchive::checkIndex(...)");
    }

private:
    fs::path file_;
    i32 entryCount_{};
    mutable uptr<wrapper::libarchive::Archive> archive_;
    mutable i32 curEntry_{};
};

}
