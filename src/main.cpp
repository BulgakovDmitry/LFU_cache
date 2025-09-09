#include <stdlib.h>                  // for EXIT_FAILURE
#include <cstring>                   // for size_t, strlen, strncmp
#include <iostream>                  // for operator<<, basic_ostream, char_...
#include <myLib.hpp>                 // for BLUE, RESET, GREEN, YELLOW
#include <vector>                    // for vector
#include "../headers/cache.hpp"      // for LFU
#include "../headers/cacheDump.hpp"  // for consoleDump, graphDump
#include "../headers/cacheFunc.hpp"  // for cachePut, getKey

int main(int argc, const char* argv[]) {
    if (!argc) return EXIT_FAILURE;

    bool interface = false;
    if(argv[1] && strncmp(argv[1], "--interface", strlen("--interface")) == 0) interface = true;

    if (interface) std::cout << BLUE << "Please enter the cache size:" << RESET;
    
    std::size_t cacheSize = 0;
    std::cin >> cacheSize;

    if (interface) std::cout << BLUE << "Please enter the number of items:" << RESET;

    std::size_t nItems = 0;
    std::cin >> nItems;

    LFU cache(cacheSize);

    std::vector<int> vec = {};
    
    if (interface) std::cout << BLUE << "Please enter " << GREEN << nItems << BLUE " items:" << RESET;
    
    int value = 0;
    for (std::size_t i = 0; i < nItems; ++i) {
        std::cin >> value;
        vec.push_back(value);
    }

    for (std::size_t i = 0; i < nItems; ++i)
        cachePut(cache, getKey(vec[i]), vec[i]);

    if (interface) {
        consoleDump(cache);
        graphDump(vec, cacheSize);
        std::cout << BLUE << "out: " << YELLOW << cache.getNumberOfHits() << std::endl << RESET; 
    }
    else
        std::cout << cache.getNumberOfHits() << std::endl; 

}
