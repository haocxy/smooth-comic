#include "libarchive-wrapper.h"

#include <optional>
#include <stdexcept>
#include <sstream>

#include <archive.h>
#include <archive_entry.h>


namespace {

class ArchiveErr : public std::exception {
public:
    ArchiveErr(const std::string &funcName, std::optional<int> err = std::nullopt) : std::exception(mkErrMsg(funcName, err).c_str()) {}
    virtual ~ArchiveErr() {}

private:
    static std::string mkErrMsg(const std::string &funcName, std::optional<int> err) {
        std::ostringstream ss;
        ss << "LibArchive Error(func: " << funcName;
        if (err) {
            ss << ", err: " << *err;
        }
        ss << ")";
        return ss.str();
    }
};

}

namespace myapp::wrapper::libarchive {

class ArchiveImpl {
public:
    ArchiveImpl(const fs::path &file) {
        archive_ = ::archive_read_new();
        if (!archive_) {
            throw ArchiveErr("archive_read_new");
        }

        ::archive_read_support_filter_all(archive_);
        ::archive_read_support_format_all(archive_);

        int r = ::archive_read_open_filename(archive_, file.generic_string().c_str(), 10240);
        if (r != ARCHIVE_OK) {
            throw ArchiveErr("archive_read_open_filename", r);
        }
    }

    ~ArchiveImpl() {
        if (archive_) {
            int r = ::archive_read_free(archive_);
            if (r != ARCHIVE_OK) {
                // TODO 析构函数的错误如何处理？
            }
        }
    }

    bool nextEntry() {
        if (finished_) {
            return false;
        }
        int r = ::archive_read_next_header(archive_, &curEntry_);
        if (r != ARCHIVE_OK) {
            finished_ = true;
            curEntry_ = nullptr;
            return false;
        }
        return true;
    }

    u8str path() const {
        return u8str(reinterpret_cast<const char8_t *>(::archive_entry_pathname_utf8(curEntry_)));
    }

    std::string readData() {
        size_t size = ::archive_entry_size(curEntry_);
        std::string buff;
        buff.resize(size);
        auto r = ::archive_read_data(archive_, buff.data(), buff.size());
        if (r < 0) {
            throw ArchiveErr("archive_read_data", static_cast<int>(r));
        }
        return buff;
    }


private:
    ::archive *archive_{};
    ::archive_entry *curEntry_{};
    bool finished_{ false };
};

Archive::Archive(const fs::path &file)
    : impl_(new ArchiveImpl(file))
{
}

Archive::~Archive()
{
    delete impl_;
    impl_ = nullptr;
}

bool Archive::nextEntry()
{
    return impl_->nextEntry();
}

u8str Archive::path() const
{
    return impl_->path();
}

std::string Archive::readData()
{
    return impl_->readData();
}


}
