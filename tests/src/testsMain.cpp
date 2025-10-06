#include "../headers/tests.hpp"
#include "../headers/data.hpp"
#include <vector>

int main() {
    caches::CacheLFU::  testsRun(caches::CacheLFU  ::dataBase, caches::CacheLFU  ::dataBase.size()); 
    caches::CacheIdeal::testsRun(caches::CacheIdeal::dataBase, caches::CacheIdeal::dataBase.size());
}
