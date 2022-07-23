#include "cache-cleaner.h"

#include <QDebug>

#include <algorithm>

#include "engine.h"
#include "path-manager.h"


namespace myapp {

CacheCleaner::CacheCleaner(Engine &engine)
    : engine_(engine)
    , thread_([this] { loop(); })
{
}

CacheCleaner::~CacheCleaner()
{
    stopped_ = true;

    sleeper_.wakeup();
}

void CacheCleaner::loop()
{
    ThreadUtil::setNameForCurrentThread("CacheCleaner");

    while (!stopped_) {
        cleanCache();
        if (stopped_) {
            break;
        }
        sleeper_.sleep(std::chrono::seconds(60));
    }
}

void CacheCleaner::cleanCache()
{
    logger_.d << "cleanCache()";

    cleanBookCache();
}

void CacheCleaner::cleanBookCache()
{
    qDebug() << "cleanBookCache";

    const fs::path dir = engine_.pathManager().bookCacheDir();
    logger_.d << "cleanBookCache() dir: " << dir;

    if (!fs::exists(dir)) {
        return;
    }

    struct Item {
    public:
        Item(const fs::path &path)
            : path(path)
            , lastWriteTime(fs::last_write_time(path))
            , fileSize(fs::file_size(path)) {}

        bool operator<(const Item &item) const {
            return lastWriteTime < item.lastWriteTime;
        }
        
        fs::path path;
        fs::file_time_type lastWriteTime;
        uintmax_t fileSize;
    };

    std::vector<Item> items;

    for (auto &entry : fs::recursive_directory_iterator(dir)) {

        if (!entry.is_regular_file()) {
            continue;
        }

        const fs::path path = fs::absolute(entry.path());

        if (!engine_.pathManager().isCacheFile(path)) {
            continue;
        }

        items.push_back(path);
    }

    std::sort(items.begin(), items.end());

    uintmax_t sum = 0;

    for (const Item &item : items) {
        sum += item.fileSize;
    }

    const uintmax_t KB = 1024;
    const uintmax_t MB = 1024 * KB;

    const uintmax_t limit = 1024 * MB;

    for (const Item &item : items) {
        if (sum <= limit) {
            break;
        }
        std::error_code err;
        fs::remove(item.path, err);
        if (!err) {
            sum -= item.fileSize;
        }
    }
}

}
