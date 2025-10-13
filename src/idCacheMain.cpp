#include <iostream>
#include "idealCache.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t cacheSize;
    std::size_t n;

    if (!(std::cin >> cacheSize >> n)) 
        return EXIT_FAILURE;

    caches::IdealCache<int, int> cache(cacheSize);
    
    int k = 0;
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> k;
        cache.lookup_update(k);
    }

    cache.cacheRun();
    std::cout << cache.getCacheHits() << std::endl;
}