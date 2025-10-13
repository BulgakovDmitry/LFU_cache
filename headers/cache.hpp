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

namespace caches {

using Tick_t = std::size_t;

template<typename KeyType, typename ValueType> 
struct CacheCell {
    KeyType     key{};
    ValueType   value{};

    std::size_t numberOfRequests{0};
    Tick_t      lastAccessedTime{0};

    explicit CacheCell(KeyType k, ValueType v, std::size_t nReq, Tick_t lastT) 
    : key{k}, value{v}, numberOfRequests{nReq}, lastAccessedTime{lastT} {} 
};

template<typename KeyType, typename ValueType> 
class LFU {
private:
    std::size_t                              cacheSize_;
    Tick_t                                   tick_;
    std::list<CacheCell<KeyType, ValueType>> data_;

public:
    using Cell           = CacheCell<KeyType, ValueType>;
    using List           = std::list<CacheCell<KeyType, ValueType>>;
    using ListIt         = typename List::iterator;
    using ListConstIt    = typename List::const_iterator;
    using Metric_t       = std::pair<size_t, Tick_t>;
    using const_iterator = typename decltype(data_)::const_iterator;

private:
struct MetricComp { 
    bool operator()(const Metric_t& lhs, const Metric_t& rhs) const {
        if (lhs.first != rhs.first) return lhs.first < rhs.first; 
        return lhs.second < rhs.second;                           
    }
};

public:

    LFU(const LFU&) = delete;
    LFU(LFU&&) = delete;
    LFU& operator=(const LFU&) = delete;
    LFU& operator=(LFU&&) = delete;
    ~LFU() = default;

    using CacheFic     = std::map<Metric_t, KeyType, MetricComp>;
    using CacheFicIter = typename CacheFic::iterator;

    LFU(std::size_t capacity)
    : cacheSize_   (capacity)
    , tick_        (0) {}

    std::size_t getCacheSize() const noexcept { return cacheSize_;      }
    std::size_t dataSize    () const noexcept { return data_.size();    }

    ListConstIt begin() const noexcept { return data_.cbegin(); }
    ListConstIt end  () const noexcept { return data_.cend(); }

    template <typename F>
    bool lookup_update(KeyType key, F slow_get_page) { 
        if (getCacheSize() == 0) return false;

        auto it = findKeyIter(key);
        if (it != end()) {
            // ---------- HIT ---------- //
            cacheFicEraseByKey(key);
            ++it->numberOfRequests;
            it->lastAccessedTime  = nextTick();
            cacheFicPut(it->numberOfRequests, it->lastAccessedTime, key);
            splice_to_front(it);
            return true;
        }

        // ---------- MISS ---------- //
        ValueType value = slow_get_page(key);
        put_to_cache(key, value);
        return false;
    }
    
private:
    bool empty() const noexcept { return data_.empty(); }

    void push_front(const CacheCell<KeyType, ValueType>& value) { data_.push_front(value); }

    std::size_t nextTick() noexcept { return ++tick_;  }

    typename LFU<KeyType, ValueType>::ListIt
    findReplacedCellIter() {
        if (getCacheSize() == 0)         return data_.end();
        if (dataSize() < getCacheSize()) return data_.end();

        auto ficIt = getCacheFicBegin();
        if (ficIt == getCacheFicEnd())   return data_.end();

        const KeyType& victimKey = ficIt->second;
        return findKeyIter(victimKey);
    }

    void eraseIndex(const KeyType& key) { hashTable_.erase(key); }

    ListIt pushNew(const KeyType& key, const ValueType& value, Tick_t now) {
        data_.emplace_front(key, value, 1, now);
        ListIt it = data_.begin();
        putIterByKey(key, it);
        cacheFicPut(it->numberOfRequests, it->lastAccessedTime, key);
        return it;
    }

    void cacheFicPut(std::size_t numberOfRequests, Tick_t lastAccessedTime, const KeyType& key) {
        auto km = keyToMetric_.find(key);
        if (km != keyToMetric_.end()) 
            cacheFic_.erase(km->second);
        
        Metric_t m{numberOfRequests, lastAccessedTime};
        cacheFic_.emplace(m, key);
        keyToMetric_[key] = m;
    }

    void cacheFicEraseByKey(const KeyType& key) {
        auto km = keyToMetric_.find(key);
        if (km != keyToMetric_.end()) {
            cacheFic_.erase(km->second);
            keyToMetric_.erase(km);
        }
    }

    typename LFU<KeyType, ValueType>::ListIt 
    findKeyIter(const KeyType& key) { return getIterByKey(key);}

    CacheFicIter getCacheFicBegin() noexcept { return cacheFic_.begin(); }
    CacheFicIter getCacheFicEnd  () noexcept { return cacheFic_.end();   }

    void splice_to_front(ListIt it) {
        data_.splice(data_.begin(), data_, it);
    }

    void putIterByKey(const KeyType& key, ListIt it) { hashTable_[key] = it; }

    ListIt getIterByKey(const KeyType& key) {
        auto it = hashTable_.find(key);
        return (it == hashTable_.end()) ? data_.end() : it->second;
    }

    void put_to_cache(KeyType key, ValueType value) {
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
        *victim = Cell{key, value, 1, nextTick()};

        putIterByKey(key, victim);
        cacheFicPut(victim->numberOfRequests, victim->lastAccessedTime, key);

        splice_to_front(victim);
    }

private:
    std::unordered_map<
        KeyType, 
        ListIt>                              hashTable_;

    std::map<Metric_t, KeyType, MetricComp>  cacheFic_;
    std::unordered_map<KeyType, Metric_t>    keyToMetric_;
};

} // namespace caches

#endif // CACHE_STRUCT_HPP