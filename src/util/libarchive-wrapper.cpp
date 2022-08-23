#include "libarchive-wrapper.h"

#include <QDebug>
#include <QString>

#include <optional>
#include <stdexcept>
#include <sstream>

#include <archive.h>
#include <archive_entry.h>

#include "core/system.h"


namespace {

class ArchiveErr : public std::logic_error {
public:
    ArchiveErr(const std::string &funcName, std::optional<int> err = std::nullopt) : std::logic_error(mkErrMsg(funcName, err)) {}
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
    ArchiveImpl() {}

    ArchiveImpl(const fs::path &file) {
        open(file);
    }

    ~ArchiveImpl() {
        close();
    }

    void close() {
        if (archive_) {
            int r = ::archive_read_free(archive_);
            if (r != ARCHIVE_OK) {
                // TODO 析构函数的错误如何处理？
            }
        }
    }

    int archiveReadOpen(const fs::path &f) {
        switch (SystemUtil::platformType) {
        case PlatformType::Windows:
            return ::archive_read_open_filename(archive_, (const char *)f.generic_string().c_str(), 10240);
        case PlatformType::Android:
            return ::archive_read_open_filename(archive_, (const char *)f.generic_u8string().c_str(), 10240);
        default:
            throw std::logic_error("unsupported platform for archive_read_open_filename");
        }
    }

    void open(const fs::path &file) {
        close();
        archive_ = ::archive_read_new();
        if (!archive_) {
            throw ArchiveErr("archive_read_new");
        }

        ::archive_read_support_filter_all(archive_);
        ::archive_read_support_format_all(archive_);

        int r = archiveReadOpen(file);
        if (r != ARCHIVE_OK) {
            throw ArchiveErr("archive_read_open_filename", r);
        }
    }

    bool nextEntry() {
        if (finished_) {
            return false;
        }

        int r = ::archive_read_next_header(archive_, &curEntry_);
        if (r == ARCHIVE_OK || r == ARCHIVE_WARN) {
            if (r == ARCHIVE_WARN) {
                qDebug() << "Archive::nextEntry warn from archive_read_next_header: "
                    << ::archive_error_string(archive_);
            }
            return true;
        }

        finished_ = true;
        curEntry_ = nullptr;
        return false;
    }

    u32str path() const {
        switch (SystemUtil::platformType) {
        case PlatformType::Windows:
        {
            const wchar_t *wcharstr = ::archive_entry_pathname_w(curEntry_);
            if (wcharstr) {
                return QString::fromWCharArray(wcharstr).toStdU32String();
            } else {
                return u32str();
            }
            break;
        }
        case PlatformType::Android:
        {
            // TODO android平台无法取到路径
            const char *str = ::archive_entry_pathname(curEntry_);
            if (str) {
                return QString::fromUtf8(str).toStdU32String();
            } else {
                return u32str();
            }
            break;
        }
        default:
        {
            throw std::logic_error("Archive::path unimplemented for current platform");
        }
        }
    }

    bool isRegularFile() const {
        return (::archive_entry_filetype(curEntry_) & AE_IFREG);
    }

    bool isDir() const {
        return (::archive_entry_filetype(curEntry_) & AE_IFDIR);
    }

    void skipContent() {
        ::archive_read_data_skip(archive_);
    }

    Buff readContent() {
        size_t size = ::archive_entry_size(curEntry_);
        Buff buff;
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

Archive::Archive()
    : impl_(new ArchiveImpl)
{
    qDebug() << "Archive::Archive()";
}

Archive::Archive(const fs::path &file)
    : impl_(new ArchiveImpl(file))
{
}

Archive::~Archive()
{
    delete impl_;
    impl_ = nullptr;
}

void Archive::open(const fs::path &file)
{
    impl_->open(file);
}

bool Archive::nextEntry()
{
    return impl_->nextEntry();
}

u32str Archive::path() const
{
    return impl_->path();
}

bool Archive::isRegularFile() const
{
    return impl_->isRegularFile();
}

bool Archive::isDir() const
{
    return impl_->isDir();
}

Buff Archive::readContent()
{
    return impl_->readContent();
}

void Archive::skipContent()
{
    impl_->skipContent();
}


}
