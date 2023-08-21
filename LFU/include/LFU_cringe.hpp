#ifndef LFU_HPP
#define LFU_HPP

#include <cstddef>
#include <cstdint>
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
class LFU {
    struct LFU_ELEM
    {
        PageT    page;
        KeyT     key;
        uint64_t hit_count;
    };
    
    using size_type = typename std::vector<LFU_ELEM>::size_type;

    static constexpr size_t NOT_FOUND       = 0xDEADBEEF;
    static constexpr size_t MAX_HIT_COUNTS  = UINT32_MAX;

    size_type capacity_;
    long int  total_hit_count_ = 0;
    
    std::vector<LFU_ELEM> cache_;

    void SearchAndReplace (KeyT& key)
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
    bool LookupAndHandle (KeyT& key){
        size_type elem_id = FindElem(key); 
        
        if (elem_id == NOT_FOUND){
            if (IsFull())
                SearchAndReplace(key);
            else
                cache_.push_back({0, key, 0});
            return NOT_HIT;
        } else {
            cache_[elem_id].hit_count++;
            total_hit_count_++;
            
            return HIT;
        }
    }

    // Find element in cache_
    size_t FindElem (KeyT& key) const {
        for (size_t i = 0; i < cache_.size(); i++){
            if (cache_[i].key == key)
                return i;
        }

        return NOT_FOUND;
    }

    KeyT GetElementById (KeyT& key) const {
        auto result = LookupAndHandle (key);
        if (result == HIT)
            return result;
        return nullptr;
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
