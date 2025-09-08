#include "../headers/cacheFunc.hpp"
#include <myLib.hpp>

static size_t findKeyIndex         (LFU* cache, size_t key);
static size_t findReplacedCellIndex(LFU* cache);

static size_t findKeyIndex(LFU* cache, size_t key)
{
    ASSERT(cache, "cache = nullptr, impossible to find key index", stderr);

    for (size_t i = 0; i < cache->cacheSize; i++)
    {
        if (cache->data[i].emptyFlag && cache->data[i].key == key)
            return i;  
    }
    return KEY_NO_FOUND;
}

static size_t findReplacedCellIndex(LFU* cache)
{
    ASSERT(cache, "cache = nullptr, impossible to find cell index for replace", stderr);

    size_t index = cache->cacheSize;

    for (size_t i = 0; i < cache->cacheSize; i++)
    {
        if (!cache->data[i].emptyFlag) return i; // WANT TO FIND FREE CELL

        if (index == cache->cacheSize                                              ||
            cache->data[i].numberOfRequests < cache->data[index].numberOfRequests  ||
           (cache->data[i].numberOfRequests == cache->data[index].numberOfRequests &&
            cache->data[i].lastAccessedTime < cache->data[index].lastAccessedTime))
            index = i;
    }
    return index;
}

void cachePut(LFU* cache, size_t key, size_t value)
{
    ASSERT(cache, "cache = nullptr, impossible to put", stderr);
    ASSERT(key, "key = 0, it is service key",           stderr);
    size_t index = findKeyIndex(cache, key);

    if (index == KEY_NO_FOUND) 
        index = findReplacedCellIndex(cache);

    cache->data[index].key = key;
    cache->data[index].value = value;
    cache->data[index].numberOfRequests = 
        cache->data[index].emptyFlag ? cache->data[index].numberOfRequests + 1 : 1;
    cache->data[index].lastAccessedTime = ++cache->tick;
    cache->data[index].emptyFlag = true;
}