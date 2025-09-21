#ifndef CACHE_STRUCT_HPP
#define CACHE_STRUCT_HPP

#include <list>
#include <cstdlib>  
#include <cstddef>
#include <unordered_map>
#include <functional> 
#include <utility>
#include <stdexcept>
#include <map>

using Tick_t = std::size_t;

template<typename KeyType, typename ValueType> 
struct CacheCell {
    KeyType     key{};
    ValueType   value{};

    std::size_t numberOfRequests{0};
    Tick_t      lastAccessedTime{0};

    bool        emptyFlag{true};
};

template<typename KeyType, typename ValueType> 
class LFU {
public:
    const std::size_t KEY_NO_FOUND = 0;

    using Cell         = CacheCell<KeyType, ValueType>;
    using List         = std::list<Cell>;
    using ListIt       = typename List::iterator;
    using ListConstIt  = typename List::const_iterator;
    using Metric_t     = std::pair<size_t, Tick_t>;

    struct MetricComp {
        bool operator()(const Metric_t& lhs, const Metric_t& rhs) const {
            if (lhs.first != rhs.first) return lhs.first < rhs.first; 
            return lhs.second < rhs.second;                           
        }
    };

    using CacheFic     = std::map<Metric_t, KeyType, MetricComp>;
    using CacheFicIter = typename CacheFic::iterator;

    explicit LFU(std::size_t capacity)
    : cacheSize_   (capacity)
    , tick_        (0)
    , data_        ()           
    , numberOfHits_(0)
    , hashTable_   ()          
    , cacheFic_    ()           
    , keyToMetric_ () {}

    ~LFU() = default;

    void        incHit          () noexcept       { ++numberOfHits_;        }
    std::size_t getCacheSize    () const noexcept { return cacheSize_;      }
    std::size_t dataSize        () const noexcept { return data_.size ();   }
    std::size_t getNumberOfHits () const noexcept { return numberOfHits_;   }

    std::size_t nextTick        ()       noexcept { return ++tick_;         }
    bool        empty           () const noexcept { return data_.empty();   }

    void push_front(const CacheCell<KeyType, ValueType>& value) { data_.push_front(value); }

    ListIt       begin()       noexcept { return data_.begin (); }
    ListConstIt  begin() const noexcept { return data_.cbegin(); }
    ListIt       end  ()       noexcept { return data_.end   (); }
    ListConstIt  end  () const noexcept { return data_.cend  (); }

    CacheFicIter getCacheFicBegin() noexcept { return cacheFic_.begin(); }
    CacheFicIter getCacheFicEnd  () noexcept { return cacheFic_.end();   }

    void splice_to_front(ListIt it) {
        if (it != begin()) data_.splice(begin(), data_, it);
    }

    void putIterByKey(const KeyType& key, ListIt it) { hashTable_[key] = it; }

    ListIt getIterByKey(const KeyType& key) {
        auto it = hashTable_.find(key);
        return (it == hashTable_.end()) ? end() : it->second;
    }

    void eraseIndex(const KeyType& key) { hashTable_.erase(key); }

    ListIt pushNew(const KeyType& key, const ValueType& value, Tick_t now) {
        push_front(Cell{key, value, 1u, now, false});
        auto it = begin();
        putIterByKey(key, it);
        cacheFicPut(it->numberOfRequests, it->lastAccessedTime, key);
        return it;
    }

    void cacheFicPut(std::size_t numberOfRequests, Tick_t lastAccessedTime, const KeyType& key) {
        auto km = keyToMetric_.find(key);
        if (km != keyToMetric_.end()) 
            cacheFic_.erase(km->second);
        
        Metric_t m{numberOfRequests, lastAccessedTime};
        cacheFic_.insert({m, key});
        keyToMetric_[key] = m;
    }

    void cacheFicEraseByKey(const KeyType& key) {
        auto km = keyToMetric_.find(key);
        if (km != keyToMetric_.end()) {
            cacheFic_.erase(km->second);
            keyToMetric_.erase(km);
        }
    }

    KeyType getKey(const ValueType& value) {
        std::size_t hash = std::hash<ValueType>{}(value);
        return static_cast<KeyType>(hash + std::size_t{1});
    }

    typename LFU<KeyType, ValueType>::ListIt 
    findKeyIter(const KeyType& key) { return getIterByKey(key);}

    typename LFU<KeyType, ValueType>::ListIt
    findReplacedCellIter() {
        if (getCacheSize() == 0)         return end();
        if (dataSize() < getCacheSize()) return end();

        auto ficIt = getCacheFicBegin();
        if (ficIt == getCacheFicEnd())   return end();

        const KeyType& victimKey = ficIt->second;
        return findKeyIter(victimKey);
    }

    void cachePut(KeyType key, ValueType value) {
        if (getCacheSize() == 0) return;

        auto it = findKeyIter(key);
        if (it != end()) {
            incHit();
            cacheFicEraseByKey(key);
            ++it->numberOfRequests;
            it->lastAccessedTime  = nextTick();
            cacheFicPut(it->numberOfRequests, it->lastAccessedTime, key);
            splice_to_front(it);
            return;
        }

        if (dataSize() < getCacheSize()) {
            pushNew(key, value, nextTick());
            return;
        }

        auto victim = findReplacedCellIter();
        if (victim == end()) {
            pushNew(key, value, nextTick());
            return;
        }

        cacheFicEraseByKey(victim->key);

        eraseIndex(victim->key); 
        *victim = Cell{key, value, 1, nextTick(), false};

        putIterByKey(key, victim);
        cacheFicPut(victim->numberOfRequests, victim->lastAccessedTime, key);

        splice_to_front(victim);
    }

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

#endif