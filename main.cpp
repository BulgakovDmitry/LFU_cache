#include <stdio.h>
#include <stdbool.h>
#include <myLib.hpp>
#include "headers/cacheStruct.hpp"

int main()
{
    printf(BLUE"Please enter the cache size:"RESET);
    size_t cacheSize = 0;
    scanf("%zu", &cacheSize);

    printf(BLUE"Please enter the number of items:"RESET);
    size_t nItems = 0;
    scanf("%zu", &nItems);

    LFU cache = {};
    lfuCtor(&cache, cacheSize);

    lfuDtor(&cache, cacheSize);

    return 0;
}
