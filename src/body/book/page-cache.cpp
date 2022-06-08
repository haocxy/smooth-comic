#include "page-cache.h"


namespace myapp {

PageCache::PageCache(const fs::path &dbFile)
    : dbFile_(dbFile)
{
    setActorName("PageCache");
}

}

