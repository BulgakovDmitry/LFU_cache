#include <stdio.h>
#include <stdbool.h>
#include <myLib.hpp>
#include <cstdint> 
#include <vector>
#include "headers/cacheStruct.hpp"
#include "headers/cacheFunc.hpp"
#include "headers/cacheDump.hpp"

int main(int argc, const char* argv[]) {
    if (!argc) return EXIT_FAILURE;

    bool interface = false;
    if(argv[1] && strncmp(argv[1], "--interface", strlen("--interface")) == 0) interface = true;

    if (interface) std::cout << BLUE << "Please enter the cache size:" << RESET;
    
    size_t cacheSize = 0;
    std::cin >> cacheSize;

    if (interface) std::cout << BLUE << "Please enter the number of items:" << RESET;

    size_t nItems = 0;
    std::cin >> nItems;

    LFU cache = {};
    lfuCtor(cache, cacheSize);

    std::vector<int> vec = {};
    
    if (interface) std::cout << BLUE << "Please enter " << GREEN << nItems << BLUE " items:" << RESET;
    
    int value = 0;
    for (size_t i = 0; i < nItems; i++) {
        std::cin >> value;
        vec.push_back(value);
    }

    for (size_t i = 0; i < nItems; ++i)
        cachePut(&cache, getKey(vec[i]), vec[i]);

    consoleDump(cache);
    lfuDtor(cache);
}
