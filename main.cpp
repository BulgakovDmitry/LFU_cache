#include <stdio.h>
#include <stdbool.h>
#include <myLib.hpp>
#include <stdint.h> 
#include <vector.hpp>
#include "headers/cacheStruct.hpp"
#include "headers/cacheFunc.hpp"

int main(int argc, const char* argv[])
{
    if (!argc)
        return EXIT_FAILURE;

    bool interface = false;
    if(argv[1] && strcmp(argv[1], "--interface") == 0)
        interface = true;

    if (interface)
        printf(BLUE"Please enter the cache size:"RESET);
    
    size_t cacheSize = 0;
    scanf("%zu", &cacheSize);

    if (interface)
        printf(BLUE"Please enter the number of items:"RESET);

    size_t nItems = 0;
    scanf("%zu", &nItems);

    LFU cache = {};
    lfuCtor(&cache, cacheSize);

    Vector vec = {};
    vectorCtor(&vec);
    
    if (interface)
        printf(BLUE"Please enter"GREEN" %zu "RESET BLUE"items:"RESET, nItems);
    
    int value;
    for (size_t i = 0; i < nItems; i++)
    {
        scanf("%d", &value);
        vectorPush(&vec, (void*)(uintptr_t)value);
    }


    //vectorDump(vec);

    vectorDtor(&vec);
    lfuDtor(&cache);

    return 0;
}
