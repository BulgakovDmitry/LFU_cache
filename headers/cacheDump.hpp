#ifndef CACHE_DUMP_HPP
#define CACHE_DUMP_HPP

#include <myLib.hpp>
#include "cache.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void consoleDump           (const LFU& cache);
void consoleGeneralizedDump(const LFU& cache);
void consoleFullDump       (const std::vector<int>& vec, std::size_t cacheSize);

void graphDump             (const std::vector<int>& vec, std::size_t cacheSize);

const std::string DUMP_FILE_GV  = "graphDump/dump.gv";
const std::string DUMP_FILE_PNG = "graphDump/dump.png";


#endif