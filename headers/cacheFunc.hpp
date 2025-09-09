#ifndef CACHE_FUNC_HPP
#define CACHE_FUNC_HPP

#include "cache.hpp"
#include <cmath>

const std::size_t KEY_NO_FOUND = 0;

inline std::size_t getKey(int value) { return abs(static_cast<long>(value) + 1); };

void cachePut(LFU& cache, std::size_t key, int value);

#endif