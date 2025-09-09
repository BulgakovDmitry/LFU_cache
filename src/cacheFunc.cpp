#include "../headers/cacheFunc.hpp"
#include <list>
#include "cache.hpp"

static inline std::list<CacheCell>::iterator findKeyIter         (LFU& cache, std::size_t key); 
static inline std::list<CacheCell>::iterator findReplacedCellIter(LFU& cache);

static inline std::list<CacheCell>::iterator findKeyIter(LFU& cache, std::size_t key)  {
    for (auto it = cache.begin(); it != cache.end(); ++it) 
        if (!it->emptyFlag && it->key == key) return it;

    return cache.end();
}

static inline std::list<CacheCell>::iterator findReplacedCellIter(LFU& cache) {
    if (cache.dataSize() < cache.getCacheSize()) return cache.end();

    auto best = cache.end();
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        if (it->emptyFlag) return it; 
        if (best == cache.end()
            ||  it->numberOfRequests <  best->numberOfRequests
            || (it->numberOfRequests == best->numberOfRequests && it->lastAccessedTime < best->lastAccessedTime)) {
            best = it;
        }
    }
    return best;
}

void cachePut(LFU& cache, std::size_t key, int value) {
    if (cache.getCacheSize() == 0) return;

    auto it = findKeyIter(cache, key);
    if (it != cache.end()) {
        ++(it->numberOfRequests);
        it->lastAccessedTime = cache.nextTick();
        it->emptyFlag = false;
        cache.cacheHit();
        cache.splice(cache.begin(), cache, it);
        return;
    }

    auto replacedCellIter = findReplacedCellIter(cache);

    if (replacedCellIter == cache.end()) {   
        cache.push_front(CacheCell{key, value, 1, cache.nextTick(), false});
        cache.cacheHit();
    }
    else {
        replacedCellIter->key              = key;
        replacedCellIter->value            = value;
        replacedCellIter->numberOfRequests = 1;
        replacedCellIter->lastAccessedTime = cache.nextTick();
        replacedCellIter->emptyFlag        = false;
        cache.splice(cache.begin(), cache, replacedCellIter);
    }
}