#include "libarchive-wrapper.h"

#include <optional>
#include <stdexcept>
#include <sstream>

#include <archive.h>


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
                throw ArchiveErr("archive_read_free", r);
            }
        }
    }


private:
    ::archive *archive_{};
};

Archive::Archive(const fs::path &file)
{
}

Archive::~Archive()
{
}


}
