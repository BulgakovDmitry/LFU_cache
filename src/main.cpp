#include <cstdlib>                 
#include <cstring>                   
#include <iostream>                 
#include "colors.hpp"               
#include <vector>                    
#include "cache.hpp"      
#include "cacheDump.hpp"  
#include "idealCache.hpp"
#include <string_view>
#include "slow_get_page.hpp"

int main(int argc, const char* argv[]) {
    bool interface = (argc > 1) && (std::string_view(argv[1]) == "--interface");

    if (interface) 
        std::cout << BLUE << "Please enter the cache size:" << RESET;
    
    std::size_t cacheSize = 0;
    std::cin >> cacheSize;

    if (interface) std::cout << BLUE << "Please enter the number of items:" << RESET;

    std::size_t nItems = 0;
    std::cin >> nItems;

    caches::LFU<std::size_t, int> cache(cacheSize);

    std::vector<int> vec = {};
    
    if (interface) std::cout << BLUE << "Please enter " << GREEN << nItems << BLUE " items:" << RESET;
    
    int value = 0;
    for (std::size_t i = 0; i < nItems; ++i) {
        std::cin >> value;
        vec.push_back(value);
    }

    std::size_t cacheHits = 0;

    for (std::size_t i = 0; i < nItems; ++i) {
        if (cache.lookup_update(vec[i], slow_get_page))
            cacheHits++;
    }

    if (interface) {
        consoleDump(cache);

        caches::graphDump<std::size_t, int>(vec, cacheSize);
        std::cout << BLUE << "out: " << YELLOW << cacheHits << std::endl << RESET; 
    }
    else
        std::cout << cacheHits << std::endl; 
}
