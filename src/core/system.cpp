#include "system.h"

#include <cstring>
#include <memory>
#include <stdexcept>

#include <boost/dll/runtime_symbol_info.hpp>

#include <QDir>

#ifdef ANDROID
#include <QJniObject>
#endif


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
    return fs::absolute(QDir::homePath().toStdU32String());
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

fs::path defaultOpenFileDir()
{
    if constexpr (platformType == PlatformType::Windows) {
        return userHome();
    }

#ifdef ANDROID
    if constexpr (platformType == PlatformType::Android) {
        QJniObject externalBaseDir = QJniObject::callStaticObjectMethod(
            "com/smoothapplication/smoothcomic/MyAppUtil",
            "getExternalBaseDir",
            "()Ljava/lang/String;");

        return fs::absolute(externalBaseDir.toString().toStdU32String());
    }
#endif

    throw std::logic_error("System::defaultOpenFileDir: unsupported platform");
}

bool isWindowedPlatform()
{
    switch (platformType) {
    case PlatformType::Windows:
        return true;
    case PlatformType::Android:
        return false;
    default:
        return false;
    }
}

}
