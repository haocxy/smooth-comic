#include "random-access-archive.h"


namespace myapp {

static i32 getEntryCount(const fs::path &file)
{
    wrapper::libarchive::Archive archive(file);

    i32 count = 0;

    while (archive.nextEntry()) {
        ++count;
        archive.skipContent();
    }

    return count;
}

void RandomAccessArchive::open(const fs::path &file)
{
    close();

    file_ = file;

    entryCount_ = getEntryCount(file);

    archive_ = std::make_unique<wrapper::libarchive::Archive>(file_);
}

void RandomAccessArchive::close()
{
    curEntry_ = 0;
    archive_ = nullptr;
    entryCount_ = 0;
    file_.clear();
}

RandomAccessArchive::Entry RandomAccessArchive::entryAt(i32 index)
{
    checkIndex(index);

    seekCurEntryTo(index);



    if (archive_->nextEntry()) {
        Entry entry;
        entry.name = archive_->path();
        entry.data = archive_->readContent();

        ++curEntry_;

        return entry;
    } else {
        throw std::runtime_error(std::format("RandomAccessArchive::entryDataAt({}) failed", index));
    }
}

void RandomAccessArchive::seekCurEntryTo(i32 index)
{
    checkIndex(index);

    if (index < curEntry_) {
        archive_ = std::make_unique<wrapper::libarchive::Archive>(file_);
        curEntry_ = 0;
    }

    for (; curEntry_ < index; ++curEntry_) {
        if (archive_->nextEntry()) {
            archive_->skipContent();
        } else {
            throw std::runtime_error(std::format("RandomAccessArchive::seekCurEntryTo({}) failed, curEntry_ = {}", index, curEntry_));
        }
    }

    if (index != curEntry_) {
        throw std::logic_error(std::format("RandomAccessArchive::seekCurEntryTo({}) failed, curEntry_ = {}", index, curEntry_));
    }
}

}
