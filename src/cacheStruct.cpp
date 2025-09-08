#include "../headers/cacheStruct.hpp"
#include <stdlib.h>
#include <myLib.hpp>

void lfuCtor(LFU* cache, size_t cacheSize)
{
    ASSERT(cache,     "cache = nullptr, impossible to ctor",      stderr);
    ASSERT(cacheSize, "cacheSize = 0, an empty cache is useless", stderr);

    cache->data = (CacheCell*)calloc(cacheSize, sizeof(CacheCell));
    ASSERT(cache->data, "cache->data = nullptr, alloc error", stderr);

    for (size_t i = 0; i < cacheSize; i++)
        cache->data[i].emptyFlag = false;

    cache->tick = 0;
}

void lfuDtor(LFU* cache, size_t cacheSize)
{
    ASSERT(cache,     "cache = nullptr, impossible to ctor",      stderr);
    ASSERT(cacheSize, "cacheSize = 0, an empty cache is useless", stderr);
    ASSERT(cache->data, "cache->data = nullptr, alloc error",     stderr);

    for (size_t i = 0; i < cacheSize; i++)
    {
        cache->data[i].emptyFlag        = false;
        cache->data[i].key              = 0;
        cache->data[i].lastAccessedTime = 0;
        cache->data[i].numberOfRequests = 0;
        cache->data[i].value            = 0;
    }

    FREE(cache->data);

    cache->tick = 0;
}