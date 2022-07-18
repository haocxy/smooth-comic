#include "cache-cleaner.h"




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
}

}
