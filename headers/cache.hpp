#ifndef CACHE_STRUCT_HPP
#define CACHE_STRUCT_HPP

#include <list>
#include <cstddef>

using Tick_t = std::size_t;

struct CacheCell {
    std::size_t key;
    int         value;

    std::size_t numberOfRequests;
    Tick_t      lastAccessedTime;

    bool        emptyFlag;
};

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

    inline void push_front(const CacheCell& value) { data.push_front(value); }

    inline std::list<CacheCell>::iterator       begin()       noexcept { return data.begin (); }
    inline std::list<CacheCell>::const_iterator begin() const noexcept { return data.cbegin(); }
    inline std::list<CacheCell>::iterator       end  ()       noexcept { return data.end   (); }
    inline std::list<CacheCell>::const_iterator end  () const noexcept { return data.cend  (); }

    inline void splice(std::list<CacheCell>::iterator pos, 
                       LFU& other, 
                       std::list<CacheCell>::iterator it) noexcept {
        data.splice(pos, other.data, it);
    }
private:
    std::size_t          cacheSize;
    Tick_t               tick;
    std::list<CacheCell> data;
    std::size_t          numberOfHits;
};

#endif