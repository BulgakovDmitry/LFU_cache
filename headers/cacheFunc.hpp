#ifndef CACHE_FUNC_HPP
#define CACHE_FUNC_HPP

#include "cacheStruct.hpp"
#include <cmath>

const size_t KEY_NO_FOUND = 0;

inline size_t getKey(int value) { return abs(static_cast<long>(value) + 1); };
void cachePut(LFU* cache, size_t key, int value);

#endif