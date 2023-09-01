#ifndef LFU_CRINGE
#define LFU_CRINGE

#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>

namespace LFU_CACHE
{

template<typename KeyT, typename PageT>
class LFU final {
    
    struct LFU_ELEM
    {
        PageT    page;
        KeyT     key;
        long int hit_count;

    };
    
    using size_type = typename std::vector<LFU_ELEM>::size_type;
    using cache_iter =  typename std::vector<LFU_ELEM>::iterator;

    size_type capacity_;
    long int  total_hit_count_ = 0;
    
    std::vector<LFU_ELEM> cache_;

    void SearchAndReplace (const KeyT& key)
    {
        cache_iter min_elem = std::min_element(cache_.begin(), cache_.end(),
                                             [](auto& first, auto& second){return first.hit_count < second.hit_count; });
        
        cache_.erase(min_elem);

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
            return false;
        } else {
            cache_[elem->key].hit_count++;
            total_hit_count_++;
            
            return true;
        }
    }


    cache_iter FindElem (const KeyT &key) 
    {
        return std::find_if (cache_.begin(), cache_.end(), [&key](auto& elem) { return elem.key == key; });
    }


    std::pair<PageT, bool> GetElementById (const KeyT& key) const {
        auto result = LookupAndHandle (key);
        if (result == true)
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

        for (size_type i = 0; i < cache_.size(); i++){
            std::cout << i <<": Key = " << cache_[i].key << ", HitCount = " << cache_[i].hit_count << "\n";
        }

        std::cout << "------- End of dump --------------\n";   
    }
}; 

} // LFU_CACHE

#endif // #ifdef LFU_CRINGE
