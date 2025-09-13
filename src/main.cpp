#include <stdlib.h>                 
#include <cstring>                   
#include <iostream>                 
#include "../common/colors.hpp"               
#include <vector>                    
#include "../headers/cache.hpp"      
#include "../headers/cacheDump.hpp"  
#include "../headers/cacheFunc.hpp" 

#include "../headers/idealCache.hpp"

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

    LFU<std::size_t, int> cache(cacheSize);

    std::vector<int> vec = {};
    
    if (interface) std::cout << BLUE << "Please enter " << GREEN << nItems << BLUE " items:" << RESET;
    
    int value = 0;
    for (std::size_t i = 0; i < nItems; ++i) {
        std::cin >> value;
        vec.push_back(value);
    }

    if (interface) {
        IdealCache<int> icache(cacheSize, vec);
        idealCacheDump<int>(cache.getCacheSize(), vec);
    }

    for (std::size_t i = 0; i < nItems; ++i)
        cachePut(cache, getKey<std::size_t, int>(vec[i]), vec[i]);

    if (interface) {
        consoleDump(cache);
        graphDump<std::size_t, int>(vec, cacheSize);
        std::cout << BLUE << "out: " << YELLOW << cache.getNumberOfHits() << std::endl << RESET; 
    }
    else
        std::cout << cache.getNumberOfHits() << std::endl; 
}
