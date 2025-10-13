#ifndef CACHE_DUMP_HPP
#define CACHE_DUMP_HPP

#include <cstdlib>                 
#include <iostream>             
#include <string>                    
#include <vector>                
#include "colors.hpp"     
#include "cache.hpp"    
#include <filesystem>             
#include <fstream>
#include "slow_get_page.hpp"

namespace caches {

struct DumpPaths {
    std::filesystem::path gv;
    std::filesystem::path png;
};

inline DumpPaths makeDumpPaths(std::string_view basename = "dump") {
    const char* env = std::getenv("GRAPH_DUMP_DIR");      
    std::filesystem::path base = (env && *env) 
        ? std::filesystem::path(env) 
        : std::filesystem::path(PROJECT_SOURCE_DIR) / "graphDump";     

        std::filesystem::create_directories(base); 
    return {
        base / (std::string(basename) + ".gv"),
        base / (std::string(basename) + ".png")
    };
}

template<typename KeyType, typename ValueType>
void        consoleDump           (const LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType>
void        consoleGeneralizedDump(const LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType>
void        consoleFullDump       (const std::vector<ValueType>& vec, std::size_t cacheSize);

template<typename KeyType, typename ValueType>
void        graphDump             (const std::vector<ValueType>& vec, std::size_t cacheSize);

template<typename KeyType, typename ValueType>
void dumpListNodes         (std::ofstream& gv, const std::vector<ValueType>& vec, std::size_t cacheSize);

void dumpConnectNodes      (std::ofstream& gv, std::size_t vecSize);

template<typename KeyType, typename ValueType>
void consoleDump(const LFU<KeyType, ValueType>& cache) {
    std::cout << MANG << "Console dump" << RED << ": " << RESET;

    for (const auto& cell : cache) {
        std::cout << YELLOW << cell.value << RESET << ' ';
    }
        
    std::cout << std::endl;
}

template<typename KeyType, typename ValueType>
void consoleGeneralizedDump(const LFU<KeyType, ValueType>& cache) {
    std::cout << MANG   << "Console dump in format " 
              << GREEN  << "["
              << YELLOW << "value"
              << RED    << ":" 
              << YELLOW << "numberOfRequests"
              << RED    << ":" 
              << YELLOW << "lastAccessedTime"
              << GREEN  << "]"
              << RED    << ": " << RESET;

    for (auto it = cache.begin(); it != cache.end(); ++it) {
        std::cout << GREEN  << "["
                  << YELLOW << it->value 
                  << RED    << ":" 
                  << YELLOW << it->numberOfRequests 
                  << RED    << ":" 
                  << YELLOW << it->lastAccessedTime 
                  << GREEN  << "]"
                  << RESET  << " ";
    }
    std::cout << std::endl;
}

template<typename KeyType, typename ValueType>
void consoleFullDump(const std::vector<ValueType>& vec, std::size_t cacheSize) {
    LFU<KeyType, ValueType> tempCache(cacheSize, true);

    std::cout << MANG "________________________Console full dump________________________\n" RESET; 

    for (std::size_t i = 0; i < vec.size(); ++i) {
        cachePut(tempCache, getKey(vec[i]), vec[i]);
        consoleGeneralizedDump(tempCache);
    }
}

template <typename KeyType, typename ValueType>
inline void writeCacheStateToLabel(std::ofstream& gv,
                                   const LFU<KeyType, ValueType>& cache)
{
    bool first = true;
    for (const auto& cell : cache) {              
        if (!first) gv << " | ";
        gv << cell.value;                        
        first = false;
    }
    if (first) gv << "(empty)";
}

template<typename KeyType, typename ValueType>
inline void dumpListNodes(std::ofstream& gv,
                          const std::vector<ValueType>& vec,
                          std::size_t cacheSize)
{
    if (!gv) {
        throw std::runtime_error("Unable to write in gv file");
    }
    LFU<KeyType, ValueType> tempCache(cacheSize);

    for (std::size_t i = 0; i < vec.size(); ++i) {
        tempCache.lookup_update(vec[i], slow_get_page);               

        gv << "\tnode_" << i
           << " [shape=Mrecord; style=filled; fillcolor=palegreen;"
              " color=\"#000000\"; fontcolor=\"#000000\"; label=\"{ step "
           << (i + 1) << " | ";
        writeCacheStateToLabel<KeyType, ValueType>(gv, tempCache);
        gv << " }\"];\n";
    }

    gv << "\tnode_" << vec.size()
       << " [shape=Mrecord; style=filled; fillcolor=palegreen;"
          " color=\"#000000\"; fontcolor=\"#000000\"; label=\"{ end | ";
    writeCacheStateToLabel<KeyType, ValueType>(gv, tempCache);
    gv << " }\"];\n\n";
}

void dumpConnectNodes(std::ofstream& gv, std::size_t vecSize) {
    if (!gv) {
        throw std::runtime_error("Unable to write in gv file");
    }
    for (std::size_t i = 0; i < vecSize; ++i) 
        gv << "\tnode_" << i << " -> node_" << i + 1 << '\n';
}

template<typename KeyType, typename ValueType>
void graphDump(const std::vector<ValueType>& vec, std::size_t cacheSize) {
    const auto paths  = makeDumpPaths();
    const std::string gvFile  = paths.gv.string();
    const std::string pngFile = paths.png.string();

    std::ofstream gv(gvFile);
    if (!gv) {
        throw std::runtime_error("Unable to opem gv file");
    }

    gv << "digraph G {\n";
    gv << "    rankdir=LR;\n";
    gv << "    graph [margin=\"0.8,1.4\"];\n";
    gv << "    node [style=filled, fontcolor=darkblue,";
    gv << "    fillcolor=peachpuff, color=\"#252A34\", penwidth=2.5];\n";
    gv << "    bgcolor=\"lemonchiffon\";\n\n";

    dumpListNodes<KeyType, ValueType>(gv, vec, cacheSize);
    dumpConnectNodes(gv, vec.size());
    
    gv << "}\n";
    gv.close();

    std::string dotComand = "dot " + gvFile + " -Tpng -o " + pngFile;
    const int rc = std::system(dotComand.c_str());
    if (rc != 0) {
        throw std::runtime_error("Graphviz 'dot' failed");
    }
}

} // namespace cahes

#endif // CACHE_DUMP_HPP