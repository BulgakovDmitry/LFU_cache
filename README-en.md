<div align="center">
  
  # LFU Cache Algorithm Implementation in C++
  ![GitHub code size](https://img.shields.io/github/languages/code-size/BulgakovDmitry/LFU_cache)  
</div>

## Other Languages

1. [Russian](/README.md)
2. [English](/README-en.md)

## Installation and Build:

To install, compile, and run without an interface, enter:
```cpp
git clone https://github.com/BulgakovDmitry/LFU_cache
cd LFU_cache
cmake -S . -B build
cd build
cmake --build .
./lfuCache 
```

To compile and run with an interface (with the flag --interface), replace make make run_LFU with:
```cpp
./lfuCache --interface
```

To compile and run the ideal cache, enter
```cpp
./idCache 
```

The program also includes a built-in testing system, which provides detailed logs in case of an error.

To run the built-in tests, enter:
```cpp
./test
```

To view detailed information about the main purposes of make, you can enter:
```cpp
make help
```

## This is important
The program implements a detailed sequential graphical dump of the algorithm using graphviz.

To quickly install graphviz on Linux, enter the following command line:
```bash
sudo apt update
sudo apt install graphviz
```

## About Dump
With its help, you can obtain the following information: let's start our cache for int type data:

<div align="center">
  <img src="docs/entering.png" alt="Entering Banner" width="400">
</div>

The graphic dump will generate the following png image:

<div align="center">
  <img src="docs/dump.png" alt="Dump Banner" width="1200">
</div>

Other console dumps have also been implemented for detailed information during debugging and selection of a convenient format (depending on the debugging object).
```cpp
template<typename KeyType, typename ValueType>
void        consoleDump           (const LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType>
void        consoleGeneralizedDump(const LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType>
void        consoleFullDump       (const std::vector<ValueType>& vec, std::size_t cacheSize);

template<typename KeyType, typename ValueType>
void        graphDump             (const std::vector<ValueType>& vec, std::size_t cacheSize);
```

## Overview
Class definition:
```cpp
template<typename KeyType, typename ValueType> 
class LFU {
public:
    // methods
private:
    std::size_t                              cacheSize_;
    Tick_t                                   tick_;
    std::list<CacheCell<KeyType, ValueType>> data_;
    std::size_t                              numberOfHits_;
    std::unordered_map<
        KeyType, 
        ListIt>                              hashTable_;

    std::map<Metric_t, KeyType, MetricComp>  cacheFic_;
    std::unordered_map<KeyType, Metric_t>    keyToMetric_;
};
```
Where cacheCell is defined as follows:
```cpp
template<typename KeyType, typename ValueType> 
struct CacheCell {
    KeyType     key;
    ValueType   value;

    std::size_t numberOfRequests;
    Tick_t      lastAccessedTime;

    bool        emptyFlag;
};
```

Explanation of the basic principles of the algorithm:

| variable       | Meaning                        |
|-------------------------|-----------------------------------|
| `cacheSize`             | number of cells in the cache     |
| `tick`             | global cache variable characterizing the algorithm's running time (in terms of the number of calls)       |
| `data`        | list of cache cells with data |
| `key`        | cached value key   |
| `value`      | cached value       |
| `numberOfRequests`      | number of repeat requests       |
| `lastAccessedTime`      | time in call units, characterizing how long it has been since the current cache cell was accessed       |
| `emptyFlag`      | number of cache hits    |
| `hashTable`   | hash table for fast searching of element by key |

## Comparison of the selected caching algorithm with the ideal one
To evaluate the efficiency of the LFU caching algorithm, an ideal caching algorithm (that "knows the future") was also implemented.
Both algorithms were given the same input data:
| Data                                                                 | LFU Cache (hits) | Ideal Cache (hits) |
| -------------------------------------------------------------------- | ---------------- | ------------------ |
| 3 5 1 2 3 4 5                                                        | 0                | 0                  |
| 3 8 1 2 3 1 2 1 4 5                                                  | 3                | 3                  |
| 1 12 8 9 10 9 4 12 8 5 3 3 13 13                                     | 2                | 3                  |
| 4 12 1 2 3 4 1 2 5 1 2 4 3 4                                         | 6                | 7                  |
| 5 30 4 2 1 2 5 4 1 6 3 2 10 2 9 2 7 5 10 2 6 1 0 1 2 4 10 5 9 10 2 5 | 12               | 18                 |

## Project Structure
```
LFU_cache/
├── headers/            # Header files
├── src/                # Source code
│
├── tests/              # built-in program testing
│   ├── src/            # test source code
│   └── headers/        # test header files
│
├── graphDump/          # for images and graphviz files
│
├── build/              # Project build folder
│   ├── obj/            # object files
│   └── bin/            # executable files
│
├── common/             # common files
│
└── Makefile/           # Makefile           
```

