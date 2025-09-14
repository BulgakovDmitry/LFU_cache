#ifndef CACHE_FUNC_HPP
#define CACHE_FUNC_HPP

#include <cstdlib>         
#include <list>            
#include "cache.hpp"   
#include <functional>    

const std::size_t KEY_NO_FOUND = 0;

template<typename KeyType, typename ValueType>
inline KeyType getKey(const ValueType& value) {
    std::size_t hash = std::hash<ValueType>{}(value);
    return static_cast<KeyType>(hash + std::size_t{1});
}


template<typename KeyType, typename ValueType>
void cachePut(LFU<KeyType, ValueType>& cache, KeyType key, ValueType value);

template<typename KeyType, typename ValueType>
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findKeyIter(LFU<KeyType, ValueType>& cache, const KeyType& key);

template<typename KeyType, typename ValueType>
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findReplacedCellIter(LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType> 
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findKeyIter(LFU<KeyType, ValueType>& cache, const KeyType& key)  {
    auto it = cache.find(key);
    if (it != cache.end()) {
        if (!it->emptyFlag && it->key == key)
             return it;
    }

    for (auto scan = cache.begin(); scan != cache.end(); ++scan) {
        if (!scan->emptyFlag && scan->key == key) {
            cache.indexKeyIt(key, scan);   
            return scan;
        }
    }

    cache.eraseKey(key);
    return cache.end();
}

template<typename KeyType, typename ValueType>
static inline typename std::list<CacheCell<KeyType, ValueType>>::iterator 
findReplacedCellIter(LFU<KeyType, ValueType>& cache) {
    if (cache.getCacheSize() == 0)               return cache.end();
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

        cache.indexKeyIt(key, it);
        
        if (cache.getInterfaceFlag())
            cache.splice(cache.begin(), cache, it);

        cache.cacheHit();
        return;
    }

    auto replacedCellIter = findReplacedCellIter(cache);

    if (replacedCellIter == cache.end()) {
        cache.push_front(CacheCell<KeyType, ValueType>{key, value, 1, cache.nextTick(), false});
        auto newIt = cache.begin();
        cache.indexKeyIt(key, newIt);
    } else {
        if (!replacedCellIter->emptyFlag) 
            cache.eraseKey(replacedCellIter->key);

        replacedCellIter->key              = key;
        replacedCellIter->value            = value;
        replacedCellIter->numberOfRequests = 1;
        replacedCellIter->lastAccessedTime = cache.nextTick();
        replacedCellIter->emptyFlag        = false;

        if (cache.getInterfaceFlag()) 
            cache.splice(cache.begin(), cache, replacedCellIter);
            
        cache.indexKeyIt(key, replacedCellIter);
    }
}

#endif