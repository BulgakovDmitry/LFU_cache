#ifndef CACHE_STRUCT_HPP
    #define CACHE_STRUCT_HPP

#include <stdio.h>
#include <stdbool.h>

typedef size_t Tick_t ;

struct CacheCell
{
    size_t key;
    int    value;

    size_t numberOfRequests;
    Tick_t lastAccessedTime;

    bool   emptyFlag;
};

struct LFU
{
    CacheCell* data;
    Tick_t     tick;
};

void lfuCtor(LFU* cache, size_t cacheSize);
void lfuDtor(LFU* cache, size_t cacheSize);

#endif