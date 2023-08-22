#ifndef LFU_HPP
#define LFU_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
#include <array>
#include <iostream>
#include <limits.h>
#include <stdint.h>


namespace LFU_CACHE
{

enum FIND_STATE
{
    HIT,
    NOT_HIT
};


template<typename PageT, typename KeyT>
class LFU final {
    
    struct LFU_ELEM
    {
        PageT    page;
        KeyT     key;
        long int hit_count;
    };
    
    using size_type = typename std::vector<LFU_ELEM>::size_type;
    using vec_iter =  typename std::vector<LFU_ELEM>::iterator;

    static constexpr size_t NOT_FOUND       = 0xDEADBEEF;
    static constexpr size_t MAX_HIT_COUNTS  = UINT32_MAX;

    size_type capacity_;
    long int  total_hit_count_ = 0;
    
    std::vector<LFU_ELEM> cache_;

    void SearchAndReplace (const KeyT& key)
    {
        size_t min_hits  = MAX_HIT_COUNTS;
        size_type min_index = NOT_FOUND;

        // Search in whole vector, because we 100% sure, that it's full.
        for (size_t i = 0; i < capacity_; i++){
            if (cache_[i].hit_count < min_hits){
                min_hits = cache_[i].hit_count;
                min_index = i;
            }
        }

        if (min_index == NOT_FOUND)
            std::cerr << "Error in Search during replace attempt\n";
        
        LFU_ELEM tmp_elem = {
            .key = key 
        };

        cache_[min_index] = tmp_elem;
    }

public:
    LFU (size_type capacity) : capacity_(capacity) {
        cache_.reserve(capacity);
    }

    // Lookup for elem in cache. In case of miss - add new elem and 
    // replace least used elem with new one if needed.
    bool LookupAndHandle (const KeyT& key){
        auto elem = FindElem(key); 
        
        if (elem == cache_.end()){
            if (IsFull())
                SearchAndReplace(key);
            else
                cache_.emplace_back(0, key, 0);
            return NOT_HIT;
        } else {
            cache_[elem->key].hit_count++;
            total_hit_count_++;
            
            return HIT;
        }
    }

    // Still in progress
    vec_iter FindElem (const KeyT& key) const {
        // Iterating like real gangsters
        return std::find_if (cache_.begin(), cache_.end(), [&key](const LFU_ELEM& elem) { return elem.key == key; });
    }

    std::pair<PageT, bool> GetElementById (const KeyT& key) const {
        auto result = LookupAndHandle (key);
        if (result == HIT)
            return std::make_pair (cache_[result].page, true);
        // Well, I don't now which element to return for better clarity.
        return std::make_pair (cache_[0].page, false);
    }

    // Checks if cache can't fit more elems inside
    bool IsFull () const {
        return capacity_ == cache_.size();
    } 


    long int GetHitcountInfo () const {
        return total_hit_count_;
    }


    void Dump () const {
        std::cout << "------- Dump of Class: LFU -------\n";
        std::cout << "Capacity: " << capacity_     << "\n";
        std::cout << "Size: "     << cache_.size() << "\n";

        std::cout << "Elements in cache: \n";

        for (size_t i = 0; i < cache_.size(); i++){
            std::cout << i <<": Key = " << cache_[i].key << ", HitCount = " << cache_[i].hit_count << "\n";
        }

        std::cout << "------- End of dump --------------\n";   
    }
}; 

};

#endif
