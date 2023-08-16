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

const uint32_t NOT_FOUND       = -1;
const uint32_t MAX_HIT_COUNTS  = UINT32_MAX;

enum FIND_STATE
{
    HIT     = true,
    NOT_HIT = false
};


template<typename PageT, typename KeyT, int ArraySize>
class LFU {

private:
    const size_t capacity_;
    uint64_t     total_hit_count_ = 0;
    
    struct LFU_ELEM
    {
        PageT    page;
        KeyT     key;
        uint64_t hit_count;
    };

    std::vector<LFU_ELEM> cache_;

    // Add new element to the end of vector 
    void PushBackElem (KeyT key){
        LFU_ELEM tmp_elem = {
            .key = key,
            .hit_count = 0
        };

        cache_.push_back(tmp_elem);    
    }

public:

    LFU (size_t capacity) : capacity_(capacity) {
        cache_.reserve(capacity);
    }
    ~LFU () {}

    // Lookup for elem in cache. In case of miss - add new elem and 
    // replace least used elem with new one if needed.
    bool LookupAndHandle (KeyT key){
        
        size_t elem_id = FindElem(key); 
        
        if (elem_id == NOT_FOUND){
            if (IsFull())
                SearchAndReplace(key);
            else
                PushBackElem(key);
            return NOT_HIT;
        } else {
            cache_[elem_id].hit_count++;
            total_hit_count_++;
            
            return HIT;
        }
    }

    void SearchAndReplace (KeyT key)
    {
        size_t min_hits  = MAX_HIT_COUNTS;
        size_t min_index = NOT_FOUND;

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

    // Find element in cache_
    size_t FindElem (KeyT key) const {
        for (size_t i = 0; i < cache_.size(); i++){
            if (cache_[i].key == key)
                return i;
        }

        return NOT_FOUND;
    }

    // Checks if cache can't fit more elems inside
    bool IsFull () const {
        return capacity_ == cache_.size();
    } 


    uint64_t GetHitcountInfo () const {
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
