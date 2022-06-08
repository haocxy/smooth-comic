#include "page-cache.h"


namespace myapp {

ImgCache::ImgCache(const fs::path &dbFile)
    : dbFile_(dbFile)
{
    setActorName("ImgCache");
}

}

