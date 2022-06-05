#include "system.h"

#include <cstring>
#include <memory>
#include <stdexcept>

#include <boost/process/environment.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

#if defined(WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#endif



namespace
{

class Initer {
public:
	Initer() {
		init();
	}

	i32 pageSize() const {
		return pageSize_;
	}

	i32 procCount() const {
		return procCount_;
	}

private:

#if defined(WIN32)
	void init() {
		SYSTEM_INFO systemInfo;
		std::memset(&systemInfo, 0, sizeof(systemInfo));
		GetSystemInfo(&systemInfo);
		pageSize_ = static_cast<i32>(systemInfo.dwPageSize);
		procCount_ = static_cast<i32>(systemInfo.dwNumberOfProcessors);
	}
#else
	void init() {
		pageSize_ = static_cast<i32>(sysconf(_SC_PAGE_SIZE));
		procCount_ = static_cast<i32>(get_nprocs());
	}
#endif

private:
	i32 pageSize_ = 0;
	i32 procCount_ = 0;
};

static const Initer g_initer;

}

static fs::path userHomeForWindows() {
    const boost::process::native_environment e = boost::this_process::environment();
    const auto driveIt = e.find("HOMEDRIVE");
    if (driveIt == e.end()) {
        throw std::runtime_error("Environment 'HOMEDRIVE' not found");
    }
    const auto pathIt = e.find("HOMEPATH");
    if (pathIt == e.end()) {
        throw std::runtime_error("Environment 'HOMEPATH' not found");
    }
    return fs::absolute(driveIt->to_string() + pathIt->to_string()).lexically_normal();
}

static fs::path userHomeForNotWindows() {
    const boost::process::native_environment e = boost::this_process::environment();
    const auto homeIt = e.find("HOME");
    if (homeIt == e.end()) {
        throw std::runtime_error("Environment 'HOME' not found");
    }
    return fs::absolute(homeIt->to_string()).lexically_normal();
}


namespace SystemUtil
{

i32 pageSize()
{
	return g_initer.pageSize();
}

i32 processorCount() {
	return g_initer.procCount();
}

fs::path userHome() {
#if defined(WIN32)
    return userHomeForWindows();
#else
    return userHomeForNotWindows();
#endif
}

static void fontsInDirForWindows(const fs::path &dir, std::vector<fs::path> &fonts)
{
    if (!fs::is_directory(dir)) {
        return;
    }

    for (const fs::directory_entry &e : fs::directory_iterator(dir)) {
        if (e.is_regular_file()) {
            fonts.push_back(fs::absolute(e.path()));
        }
    }
}

fs::path exePath()
{
    return fs::absolute(boost::dll::program_location().generic_wstring());
}

}
