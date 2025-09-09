#include "../headers/cacheFunc.hpp"
#include <myLib.hpp>

static inline std::list<CacheCell>::iterator findKeyIter         (LFU* cache, size_t key); 
static inline std::list<CacheCell>::iterator findReplacedCellIter(LFU* cache);

static inline std::list<CacheCell>::iterator findKeyIter(LFU* cache, size_t key)  {
    for (auto it = cache->data.begin(); it != cache->data.end(); ++it) 
        if (!it->emptyFlag && it->key == key) return it;

    return cache->data.end();
}

static inline std::list<CacheCell>::iterator findReplacedCellIter(LFU* cache) {
    if (cache->data.size() < cache->cacheSize) return cache->data.end();

    auto best = cache->data.end();
    for (auto it = cache->data.begin(); it != cache->data.end(); ++it) {
        if (it->emptyFlag) return it; 
        if (best == cache->data.end()
            || it->numberOfRequests < best->numberOfRequests
            || (it->numberOfRequests == best->numberOfRequests && it->lastAccessedTime < best->lastAccessedTime)) {
            best = it;
        }
    }
    return best;
}

void cachePut(LFU* cache, size_t key, int value) {
    ASSERT(cache, "cache = nullptr, impossible to put", stderr);

    if (cache->cacheSize == 0) return;

    auto it = findKeyIter(cache, key);
    if (it != cache->data.end()) {
        ++(it->numberOfRequests);
        it->lastAccessedTime = cache->nextTick();
        it->emptyFlag = false;
        cache->data.splice(cache->data.end(), cache->data, it);
        return;
    }

    auto replacedCellIter = findReplacedCellIter(cache);

    if (replacedCellIter == cache->data.end()) 
        cache->data.push_back(CacheCell{key, value, 1, cache->nextTick(), false});
    else {
        replacedCellIter->key              = key;
        replacedCellIter->value            = value;
        replacedCellIter->numberOfRequests = 1;
        replacedCellIter->lastAccessedTime = cache->nextTick();
        replacedCellIter->emptyFlag        = false;
        cache->data.splice(cache->data.end(), cache->data, replacedCellIter);
    }
}