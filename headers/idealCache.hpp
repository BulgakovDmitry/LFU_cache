#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <cstddef>
#include <limits>
#include <map>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename KeyType, typename ValueType>
class IdealCache {
public:
    explicit IdealCache(std::size_t cacheSize)
        : values_          ()           
        , cacheSize_       (cacheSize)
        , cacheHits_       (0)
        , nextOccurrences_ ()           
        , nextUse_         ()
        , futureQueue_     ()          
    {
        values_.reserve(64);           
    }

    void lookupUpdate(const KeyType& key);

    void cacheRun();

    std::size_t getCacheHits() const { return cacheHits_; }

private:
    std::vector<KeyType> values_;
    std::size_t cacheSize_;
    std::size_t cacheHits_;

    std::unordered_map<KeyType, std::vector<ValueType>> nextOccurrences_;
    std::unordered_map<KeyType, ValueType>              nextUse_;
    std::multimap<ValueType, KeyType>                   futureQueue_;
};

template <typename KeyType, typename ValueType>
void IdealCache<KeyType, ValueType>::lookupUpdate(const KeyType& key) {
    values_.push_back(key);
}

template <typename KeyType, typename ValueType>
void IdealCache<KeyType, ValueType>::cacheRun() {
    cacheHits_ = 0;
    if (cacheSize_ == 0 || values_.empty()) {
        nextOccurrences_.clear();
        nextUse_.clear();
        futureQueue_.clear();
        return;
    }

    nextOccurrences_.clear();
    nextUse_.clear();
    futureQueue_.clear();

    auto eraseOne = [&](ValueType when, const KeyType& k) {
        auto range = futureQueue_.equal_range(when);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == k) { futureQueue_.erase(it); break; }
        }
    };

    for (int i = static_cast<int>(values_.size()) - 1; i >= 0; --i) 
        nextOccurrences_[values_[static_cast<std::size_t>(i)]].push_back(static_cast<ValueType>(i));

    const ValueType INFTY = std::numeric_limits<ValueType>::max();

    for (std::size_t t = 0; t < values_.size(); ++t) {
        const KeyType& key = values_[t];

        auto& occ = nextOccurrences_[key];
        if (!occ.empty()) occ.pop_back();

        ValueType next = INFTY;
        if (!occ.empty()) next = occ.back();

        auto it = nextUse_.find(key);
        if (it != nextUse_.end()) {
            // ---------------- HIT ----------------
            ++cacheHits_;
            ValueType oldNext = it->second;

            if (next != INFTY) {
                eraseOne(oldNext, key);
                it->second = next;
                futureQueue_.emplace(next, key);
            } else {
                eraseOne(oldNext, key);
                nextUse_.erase(it);
            }
        } else {
            // ---------------- MISS ---------------
            if (next == INFTY) {
                continue;
            }

            nextUse_.emplace(key, next);
            futureQueue_.emplace(next, key);

            if (nextUse_.size() > cacheSize_) {
                auto last = std::prev(futureQueue_.end()); 
                const KeyType victim = last->second;
                futureQueue_.erase(last);
                nextUse_.erase(victim);
            }
        }
    }
}

} // namespace caches

#endif // IDEAL_CACHE_HPP