<div align="center">
  
  # 🛡️ Implementation of the LFU caching algorithm in C++
  
  ![GitHub code size](https://img.shields.io/github/languages/code-size/BulgakovDmitry/readmeRepo)  
  
  <img src="docs/LFUPicREADME.png" alt="LFU Banner" width="800">
  
</div>

## Other languages

1. [Russian](/README.md)
2. [English](/README-en.md)

## 🚀 Quick Start:

To download, compile and run the program with the “--interface” flag, enter:
```cpp
git clone https://github.com/BulgakovDmitry/LFU_cache
cd LFU_cache
make run_with_interface
```
To compile and run without an interface, replace make run_with_interface with:
```cpp
make run
```

The program also features a convenient testing system for this algorithm, which provides detailed logs in case of errors.
To run the built-in tests:
```cpp
make test
```

To view detailed information about the main purposes of make, you can enter:
```cpp
make help
```

## ✨ This is important
The program implements a detailed sequential graphical dump of the algorithm using graphviz.

To quickly install graphviz on Linux, enter the following command line:
```bash
sudo apt update
sudo apt install graphviz
```

## 💡 About Dump
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

## 🔍 Overview
Class definition:
```cpp
template<typename KeyType, typename ValueType> 
class LFU {
public:
    explicit LFU(std::size_t cacheSize_) noexcept
        : cacheSize(cacheSize_), tick(0), data{}, numberOfHits(0), hashTable{} {}

    ~LFU() = default;

    using ListIt      = typename std::list<CacheCell<KeyType, ValueType>>::iterator;
    using ListConstIt = typename std::list<CacheCell<KeyType, ValueType>>::const_iterator;

    inline void        cacheHit       ()       noexcept {        ++numberOfHits; }
    inline std::size_t nextTick       ()       noexcept { return ++tick;         }
    inline std::size_t getNumberOfHits() const noexcept { return numberOfHits;   }
    inline std::size_t getCacheSize   () const noexcept { return cacheSize;      }
    inline std::size_t dataSize       () const noexcept { return data.size ();   }
    inline bool        empty          () const noexcept { return data.empty();   }

    inline void push_front(const CacheCell<KeyType, ValueType>& value) { data.push_front(value); }

    inline ListIt       begin()       noexcept { return data.begin (); }
    inline ListConstIt  begin() const noexcept { return data.cbegin(); }
    inline ListIt       end  ()       noexcept { return data.end   (); }
    inline ListConstIt  end  () const noexcept { return data.cend  (); }

    inline void splice(ListIt pos, LFU& other, ListIt it) noexcept { data.splice(pos, other.data, it); }

    inline ListIt      find(const KeyType& key) {
        auto hit = hashTable.find(key);
        return (hit == hashTable.end()) ? data.end() : hit->second;
    }

    inline ListConstIt find(const KeyType& key) const {
        auto hit = hashTable.find(key);
        return (hit == hashTable.end()) ? data.end() : hit->second;
    }

    inline void indexKeyIt(const KeyType& key, ListIt it) { hashTable[key] = it;  }
    inline void eraseKey  (const KeyType& key)            { hashTable.erase(key); }
    inline void eraseIt   (ListIt it) {
        if (it == data.end()) return;
        hashTable.erase(it->key);
        data.erase(it);
    }

private:
    std::size_t                              cacheSize;
    Tick_t                                   tick;
    std::list<CacheCell<KeyType, ValueType>> data;
    std::size_t                              numberOfHits;
    std::unordered_map<
        KeyType, 
        ListIt>                              hashTable;
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

## 📂 Project Structure
```
LFU/
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
├── myLib/              # my castom library
│
└── Makefile/           # Makefile           
```

