#include "book-cache.h"

#include "core/logger.h"


namespace myapp {

BookCache::BookCache(fs::path cacheDir)
    : cacheDir_(std::move(cacheDir))
{
    setActorName("BookCache");
}

}
