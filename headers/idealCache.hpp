#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <list>


template <typename KeyType, typename ValueType>
class IdealCache {
public:
    explicit IdealCache(std::size_t cacheSize) : values_{}, cacheSize_(cacheSize), cacheHits_(0) {}

    std::size_t getCacheHits() const noexcept { return cacheHits_; }
    void        incHits     ()       noexcept { cacheHits_++;      }

    size_t IdealCache<KeyType, ValueType>::cacheRun();

private:
    std::vector<KeyType> values_;
    std::size_t          cacheSize_;
    std::size_t          cacheHits_;

    std::unordered_map<KeyType, ValueType> nextUse_;
    std::map<ValueType,KeyType>            futureQueue_;
};

template <typename KeyType, typename ValueType> 
size_t IdealCache<KeyType, ValueType>::cacheRun() {

} 

#endif

