#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <ostream>
#include <iostream>
#include <cstddef>
#include <iomanip>

template <typename ValueType>
class IdealCache {
public:
    IdealCache(std::size_t cacheSize, const std::vector<ValueType>& requests)
        :   cacheSize_{cacheSize},
            requests_{requests},
            cache_{},
            numberOfHits_(0),
            occurrences_{},
            cursors_{},
            processed_{false} 
    {
        buildOccurrences_();
    }

    ~IdealCache() = default;

    inline void cacheHit() noexcept { ++numberOfHits_; }

    void process() {
        if (processed_) return;
        if (cacheSize_ == 0) {
            processed_ = true;
            return;
        }

        for (const auto& kv : occurrences_) 
            cursors_.emplace(kv.first, std::size_t{0});

        for (std::size_t i = 0; i < requests_.size(); ++i) {
            const ValueType& req = requests_[i];

            const bool hit = cache_.find(req) != cache_.end();
            if (hit) 
                cacheHit();
            else {
                if (cache_.size() >= cacheSize_) {
                    ValueType victim = chooseVictim_(i);
                    cache_.erase(victim);
                }
                cache_.insert(req);
            }

            auto& cur = cursors_[req]; 
            auto& occ = occurrences_[req];
            while (cur < occ.size() && occ[cur] <= i) {
                ++cur;
            }
        }

        processed_ = true;
    }

    void dump() {
        process();
        const std::size_t total = requests_.size();

        std::cout << MANG << "Ideal cache"       << RED " {"          << RESET << '\n';
        std::cout << MANG << "   Cache size:\t " << cacheSize_    << RESET << '\n';
        std::cout << MANG << "   Requests:\t "   << total         << RESET << '\n';
        std::cout << MANG << "   Hits:\t "       << numberOfHits_ << RESET << '\n';
        std::cout << RED  << "}"                                << RESET << std::endl;
    }

private:
    void buildOccurrences_() {
        for (std::size_t i = 0; i < requests_.size(); ++i) 
            occurrences_[requests_[i]].push_back(i);
    }

    ValueType chooseVictim_(std::size_t /*i*/) {
        ValueType best{};
        std::size_t bestNext = 0;
        bool bestInitialized = false;

        for (const ValueType& item : cache_) {
            const std::size_t nextUse = nextUseIndex_(item);
            if (!bestInitialized || nextUse > bestNext) {
                best = item;
                bestNext = nextUse;
                bestInitialized = true;
            }
        }
        return best;
    }

    std::size_t nextUseIndex_(const ValueType& item) const {
        auto itOcc = occurrences_.find(item);
        if (itOcc == occurrences_.end()) {
            return std::numeric_limits<std::size_t>::max();
        }
        auto itCur = cursors_.find(item);
        std::size_t cur = (itCur == cursors_.end()) ? 0 : itCur->second;
        const auto& vec = itOcc->second;
        if (cur >= vec.size()) 
            return std::numeric_limits<std::size_t>::max();
    
        return vec[cur];
    }

    std::size_t cacheSize_;
    std::vector<ValueType> requests_;

    std::unordered_set<ValueType> cache_;
    std::size_t numberOfHits_;

    std::unordered_map<ValueType, std::vector<std::size_t>> occurrences_;
    std::unordered_map<ValueType, std::size_t> cursors_;

    bool processed_;
};

template <typename ValueType>
void idealCacheDump(std::size_t cacheSize, const std::vector<ValueType>& requests) {
    IdealCache<ValueType> cache(cacheSize, requests);
    cache.dump();
}

#endif