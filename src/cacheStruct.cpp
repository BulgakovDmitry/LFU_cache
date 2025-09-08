#include "../headers/cacheStruct.hpp"
#include <stdlib.h>
#include <myLib.hpp>

void lfuCtor(LFU* cache, size_t cacheSize)
{
    ASSERT(cache,     "cache = nullptr, impossible to ctor",      stderr);
    ASSERT(cacheSize, "cacheSize = 0, an empty cache is useless", stderr);

    cache->tick      = 0;
    cache->cacheSize = cacheSize;
    cache->data.clear();

    CacheCell empty{};
    empty.key              = 0;
    empty.value            = 0;
    empty.numberOfRequests = 0;
    empty.lastAccessedTime = 0;
    empty.emptyFlag        = true;

    for (size_t i = 0; i < cacheSize; ++i) 
        cache->data.push_back(empty);
}

void lfuDtor(LFU* cache)
{
    ASSERT(cache, "cache = nullptr, impossible to ctor", stderr);

    listDtor(cache->data);

    cache->cacheSize = 0;
    cache->tick      = 0;
}