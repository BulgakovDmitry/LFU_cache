#ifndef CACHE_FUNC_HPP
#define CACHE_FUNC_HPP

#include <bits/std_abs.h> 
#include <cstdlib>         
#include <list>            
#include "cache.hpp"       

const std::size_t KEY_NO_FOUND = 0;

template<typename KeyType, typename ValueType>
inline KeyType getKey(ValueType value) { return std::abs(static_cast<long>(value) + 1); };

template<typename KeyType, typename ValueType>
void cachePut(LFU<KeyType, ValueType>& cache, KeyType key, ValueType value);

template<typename KeyType, typename ValueType>
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findKeyIter(LFU<KeyType, ValueType>& cache, KeyType key); 

template<typename KeyType, typename ValueType>
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findReplacedCellIter(LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType> 
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findKeyIter(LFU<KeyType, ValueType>& cache, KeyType key)  {
    for (auto it = cache.begin(); it != cache.end(); ++it) 
        if (!it->emptyFlag && it->key == key) return it;

    return cache.end();
}

template<typename KeyType, typename ValueType>
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findReplacedCellIter(LFU<KeyType, ValueType>& cache) {
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

template<typename KeyType, typename ValueType>
void cachePut(LFU<KeyType, ValueType>& cache, KeyType key, ValueType value) {
    if (cache.getCacheSize() == 0) return;

    auto it = findKeyIter(cache, key);
    if (it != cache.end()) {
        ++(it->numberOfRequests);
        it->lastAccessedTime = cache.nextTick();
        it->emptyFlag = false;
        cache.splice(cache.begin(), cache, it);
        cache.cacheHit();
        return;
    }

    auto replacedCellIter = findReplacedCellIter(cache);

    if (replacedCellIter == cache.end()) 
        cache.push_front(CacheCell<KeyType, ValueType>{key, value, 1, cache.nextTick(), false});
    else {
        replacedCellIter->key              = key;
        replacedCellIter->value            = value;
        replacedCellIter->numberOfRequests = 1;
        replacedCellIter->lastAccessedTime = cache.nextTick();
        replacedCellIter->emptyFlag        = false;
        cache.splice(cache.begin(), cache, replacedCellIter);
    }
}

#endif