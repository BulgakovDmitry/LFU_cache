#ifndef CACHE_FUNC_HPP
#define CACHE_FUNC_HPP

#include <cstdlib>         
#include <list>            
#include "cache.hpp"   
#include <functional>    
#include <map>

const std::size_t KEY_NO_FOUND = 0;

//TODO: put all this code in class methods

template<typename KeyType, typename ValueType>
inline KeyType getKey(const ValueType& value) {
    std::size_t hash = std::hash<ValueType>{}(value);
    return static_cast<KeyType>(hash + std::size_t{1});
}

template<typename KeyType, typename ValueType>
void cachePut(LFU<KeyType, ValueType>& cache, KeyType key, ValueType value);

template<typename KeyType, typename ValueType>
typename LFU<KeyType, ValueType>::ListIt
findKeyIter(LFU<KeyType, ValueType>& cache, const KeyType& key) {
    return cache.getIterByKey(key);
}

template<typename KeyType, typename ValueType>
typename LFU<KeyType, ValueType>::ListIt
findReplacedCellIter(LFU<KeyType, ValueType>& cache) {
    if (cache.getCacheSize() == 0)               return cache.end();
    if (cache.dataSize() < cache.getCacheSize()) return cache.end();

    auto ficIt = cache.getCacheFicBegin();
    if (ficIt == cache.getCacheFicEnd()) return cache.end();

    const KeyType& victimKey = ficIt->second;
    return findKeyIter<KeyType, ValueType>(cache, victimKey);
}

template<typename KeyType, typename ValueType>
void cachePut(LFU<KeyType, ValueType>& cache, KeyType key, ValueType value) {
    if (cache.getCacheSize() == 0) return;

    auto it = findKeyIter<KeyType, ValueType>(cache, key);
    if (it != cache.end()) {
        cache.incHit();
        cache.cacheFicEraseByKey(key);
        it->numberOfRequests += 1;
        it->lastAccessedTime  = cache.nextTick();
        cache.cacheFicPut(it->numberOfRequests, it->lastAccessedTime, key);
        if (cache.getInterfaceFlag()) cache.splice_to_front(it);
        return;
    }

    if (cache.dataSize() < cache.getCacheSize()) {
        cache.pushNew(key, value, cache.nextTick());
        return;
    }

    auto victim = findReplacedCellIter<KeyType, ValueType>(cache);
    if (victim == cache.end()) {
        cache.pushNew(key, value, cache.nextTick());
        return;
    }

    cache.cacheFicEraseByKey(victim->key);

    cache.eraseIndex(victim->key); 
    victim->key               = key;
    victim->value             = value;
    victim->numberOfRequests  = 1;
    victim->lastAccessedTime  = cache.nextTick();
    victim->emptyFlag         = false;

    cache.indexKeyIt(key, victim);
    cache.cacheFicPut(victim->numberOfRequests, victim->lastAccessedTime, key);

    if (cache.getInterfaceFlag()) cache.splice_to_front(victim);
}

#endif