#ifndef CACHE_DUMP_HPP
#define CACHE_DUMP_HPP

#include <myLib.hpp>
#include "cacheStruct.hpp"
#include <iostream>
#include <vector>

void consoleDump    (const LFU& cache);
void consoleFullDump(const LFU& cache);

#endif