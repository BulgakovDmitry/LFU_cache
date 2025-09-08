#ifndef CACHE_FUNC_HPP
#define CACHE_FUNC_HPP

#include "cacheStruct.hpp"

const size_t KEY_NO_FOUND = 0;

void cachePut(LFU* cache, size_t key, int value);

#endif