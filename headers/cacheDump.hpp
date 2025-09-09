#ifndef CACHE_DUMP_HPP
#define CACHE_DUMP_HPP

#include <cstddef>  
#include <string>   
#include <vector>  

class LFU;

void consoleDump           (const LFU& cache);
void consoleGeneralizedDump(const LFU& cache);
void consoleFullDump       (const std::vector<int>& vec, std::size_t cacheSize);

void graphDump             (const std::vector<int>& vec, std::size_t cacheSize);

const std::string DUMP_FILE_GV  = "graphDump/dump.gv";
const std::string DUMP_FILE_PNG = "graphDump/dump.png";


#endif