#ifndef CACHE_STRUCT_HPP
#define CACHE_STRUCT_HPP

#include <list>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <stdexcept>

using Tick_t = std::size_t;

template<typename KeyType, typename ValueType> 
struct CacheCell {
    KeyType     key;
    ValueType   value;

    std::size_t numberOfRequests;
    Tick_t      lastAccessedTime;

    bool        emptyFlag;
};

template<typename KeyType, typename ValueType> 
class LFU {
public:
    explicit LFU(std::size_t cacheSize_, bool interfaseFlag_) noexcept
        : cacheSize(cacheSize_), tick(0), data{}, numberOfHits(0), hashTable{}, interfaseFlag(interfaseFlag_) {}

    ~LFU() = default;

    using ListIt      = typename std::list<CacheCell<KeyType, ValueType>>::iterator;
    using ListConstIt = typename std::list<CacheCell<KeyType, ValueType>>::const_iterator;

    inline void        cacheHit        ()       noexcept {        ++numberOfHits; }
    inline std::size_t nextTick        ()       noexcept { return ++tick;         }
    inline std::size_t getNumberOfHits () const noexcept { return numberOfHits;   }
    inline std::size_t getCacheSize    () const noexcept { return cacheSize;      }
    inline std::size_t dataSize        () const noexcept { return data.size ();   }
    inline bool        empty           () const noexcept { return data.empty();   }
    inline bool        getInterfaceFlag() const noexcept { return interfaseFlag;  }

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
    bool                                     interfaseFlag;
};

#endif