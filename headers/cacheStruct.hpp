#ifndef CACHE_STRUCT_HPP
#define CACHE_STRUCT_HPP

#include <stdio.h> 
#include <stdbool.h>
#include <list>

typedef size_t Tick_t;

struct CacheCell {
    size_t key;
    int    value;

    size_t numberOfRequests;
    Tick_t lastAccessedTime;

    bool   emptyFlag;
};

struct LFU {
    size_t     cacheSize;

    std::list<CacheCell> data;
    Tick_t     tick;

    inline size_t nextTick() { return ++tick; }
};

template <class T>
inline void listDtor(std::list<T>& list) { list.clear(); }

void lfuCtor(LFU& cache, size_t cacheSize);
void lfuDtor(LFU& cache);

#endif