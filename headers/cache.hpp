#ifndef CACHE_STRUCT_HPP
#define CACHE_STRUCT_HPP

#include <list>
#include <cstddef>

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
    explicit LFU(std::size_t cacheSize_) noexcept
        : cacheSize(cacheSize_), tick(0), data{}, numberOfHits(0) {}

    ~LFU() = default;

    inline void        cacheHit       ()       noexcept {        ++numberOfHits; }
    inline std::size_t nextTick       ()       noexcept { return ++tick;         }
    inline std::size_t getNumberOfHits() const noexcept { return numberOfHits;   }
    inline std::size_t getCacheSize   () const noexcept { return cacheSize;      }
    inline std::size_t dataSize       () const noexcept { return data.size ();   }
    inline bool        empty          () const noexcept { return data.empty();   }

    inline void push_front(const CacheCell<KeyType, ValueType>& value) { data.push_front(value); }

    inline typename std::list<CacheCell<KeyType, ValueType>>::iterator       begin()       noexcept { return data.begin (); }
    inline typename std::list<CacheCell<KeyType, ValueType>>::const_iterator begin() const noexcept { return data.cbegin(); }
    inline typename std::list<CacheCell<KeyType, ValueType>>::iterator       end  ()       noexcept { return data.end   (); }
    inline typename std::list<CacheCell<KeyType, ValueType>>::const_iterator end  () const noexcept { return data.cend  (); }

    inline void splice(typename std::list<CacheCell<KeyType, ValueType>>::iterator pos, 
                       LFU& other, 
                       typename std::list<CacheCell<KeyType, ValueType>>::iterator it) noexcept {
        data.splice(pos, other.data, it);
    }
private:
    std::size_t                              cacheSize;
    Tick_t                                   tick;
    std::list<CacheCell<KeyType, ValueType>> data;
    std::size_t                              numberOfHits;
};

#endif