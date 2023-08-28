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
#include <iterator>

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
    //TODO implement with emplace_back
    // using page_type = std::function<PageT(const KeyT&)>; 

    size_type capacity_;
    long int  total_hit_count_ = 0;
    PageT init_page_;
    
    std::vector<LFU_ELEM> cache_;

    void SearchAndReplace (const KeyT& key)
    {
        vec_iter min_elem = std::min_element(cache_.begin(), cache_.end(),
                                             [](auto& first, auto& second){return first.hit_count < second.hit_count; });
        
        cache_.erase(min_elem);

        //TODO replace with emplace back
        cache_.push_back({0, key, 0});    
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
                cache_.push_back({0, key, 0});
            return NOT_HIT;
        } else {
            cache_[elem->key].hit_count++;
            total_hit_count_++;
            
            return HIT;
        }
    }


    vec_iter FindElem (const KeyT &key)
    {
        return std::find_if (cache_.begin(), cache_.end(), [&key](const LFU_ELEM& elem) { return elem.key == key; });
    }


    std::pair<PageT, bool> GetElementById (const KeyT& key) const {
        auto result = LookupAndHandle (key);
        if (result == HIT)
            return std::make_pair (cache_[result].page, true);
        // Not sure which element to return for better clarity.
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
