#ifndef CACHE_STRUCT_HPP
#define CACHE_STRUCT_HPP

#include <list>
#include <cstddef>
#include <unordered_map>
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

    using Cell        = CacheCell<KeyType, ValueType>;
    using List        = std::list<Cell>;
    using ListIt      = typename List::iterator;
    using ListConstIt = typename List::const_iterator;
    using Metric_t    = std::pair<size_t, Tick_t>;

    struct MetricComp {
        bool operator()(const Metric_t& lhs, const Metric_t& rhs) const {
            if (lhs.first != rhs.first) return lhs.first < rhs.first; 
            return lhs.second < rhs.second;                           
        }
    };

    using CacheFic = std::map<Metric_t, KeyType, MetricComp>;
    using CacheFicIter = typename CacheFic::iterator;

    explicit LFU(std::size_t capacity, bool interfaceFlag=false)
        : cacheSize_(capacity)
        , tick_(0)
        , numberOfHits_(0)
        , interfaseFlag_(interfaceFlag)
    {
        data_.clear();
        hashTable_.clear();
        keyToMetric_.clear();
        cacheFic_.clear();
    }

    ~LFU() = default;

    void        incHit          () noexcept       { ++numberOfHits_;       }
    std::size_t getCacheSize    () const noexcept { return cacheSize_;      }
    std::size_t dataSize        () const noexcept { return data_.size ();   }
    std::size_t getNumberOfHits () const noexcept { return numberOfHits_;   }
    bool        getInterfaceFlag() const noexcept { return interfaseFlag_;  }

    void        cacheHit        ()       noexcept {        ++numberOfHits_; }
    std::size_t nextTick        ()       noexcept { return ++tick_;         }
    bool        empty           () const noexcept { return data_.empty();   }

    void push_front(const CacheCell<KeyType, ValueType>& value) { data_.push_front(value); }

    ListIt       begin()       noexcept { return data_.begin (); }
    ListConstIt  begin() const noexcept { return data_.cbegin(); }
    ListIt       end  ()       noexcept { return data_.end   (); }
    ListConstIt  end  () const noexcept { return data_.cend  (); }

    CacheFicIter getCacheFicBegin() { return cacheFic_.begin(); }
    CacheFicIter getCacheFicEnd  () { return cacheFic_.end();   }

    void splice_to_front(ListIt it) {
        if (it != data_.begin()) data_.splice(data_.begin(), data_, it);
    }

    ListIt      find(const KeyType& key) {
        auto hit = hashTable_.find(key);
        return (hit == hashTable_.end()) ? data_.end() : hit->second;
    }

    ListConstIt find(const KeyType& key) const {
        auto hit = hashTable_.find(key);
        return (hit == hashTable_.end()) ? data_.end() : hit->second;
    }

    void indexKeyIt(const KeyType& key, ListIt it) { hashTable_[key] = it; }

    void eraseIndex(const KeyType& key) { hashTable_.erase(key); }
    
    inline void eraseIt(ListIt it) {
        if (it == data_.end()) return;
        hashTable_.erase(it->key);
        data_.erase(it);
    }

    ListIt getIterByKey(const KeyType& key) {
        auto it = hashTable_.find(key);
        return (it == hashTable_.end()) ? data_.end() : it->second;
    }

    ListIt pushNew(const KeyType& key, const ValueType& value, Tick_t now) {
        data_.push_front(Cell{key, value, 1u, now, false});
        auto it = data_.begin();
        indexKeyIt(key, it);
        cacheFicPut(it->numberOfRequests, it->lastAccessedTime, key);
        return it;
    }

    void cacheFicPut(std::size_t numberOfRequests, Tick_t lastAccessedTime, const KeyType& key) {
        auto km = keyToMetric_.find(key);
        if (km != keyToMetric_.end()) {
            cacheFic_.erase(km->second);
        }
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

private:
    std::size_t                              cacheSize_;
    Tick_t                                   tick_;
    std::list<CacheCell<KeyType, ValueType>> data_;
    std::size_t                              numberOfHits_;
    std::unordered_map<
        KeyType, 
        ListIt>                              hashTable_;
    bool                                     interfaseFlag_;

    std::map<Metric_t, KeyType, MetricComp> cacheFic_;
    std::unordered_map<KeyType, Metric_t> keyToMetric_;
};

#endif