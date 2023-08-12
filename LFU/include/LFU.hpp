#ifndef LFU_HPP
#define LFU_HPP

#include <cstddef>
#include <vector>
#include <iostream>


namespace LFU_DODO
{

const size_t NOT_FOUND       = -1;
const size_t MAX_HIT_COUNTS  = 10000;

template<typename PageT, typename KeyT>
class LFU {

private:
    size_t capacity_;

    struct lfu_elem
    {
        PageT  page;
        KeyT   key;
        size_t hit_count;
    };

    std::vector<lfu_elem> cache_;
    

public:    
    LFU (size_t capacity) : capacity_(capacity) {}

    // Lookup for elem in cache. In case of miss - add new elem and 
    // replace least used elem with new one if needed.
    bool LookupAndHandle (KeyT key){
        
        size_t elem_id = FindElem(key); 
        
        if (elem_id == NOT_FOUND){
            if (IsFull()){
                std::cout << "List is full, replacing element:\n";
                SearchAndReplace(key);
            } else {
                std::cout << "New element, adding to the end of vector\n";
                PushBackElem(key);
            }
        } else {
            std::cout << "Hit!\n";
            cache_[elem_id].hit_count++;
        }

        return true;
    }

    void SearchAndReplace (KeyT key)
    {
        size_t min_hits     = MAX_HIT_COUNTS;
        size_t min_index = NOT_FOUND;

        // Search in whole vector, because we 100% sure, that it's full.
        for (size_t i = 0; i < capacity_; i++)
        {
            if (cache_[i].hit_count < min_hits){
                min_hits = cache_[i].hit_count;
                min_index = i;
            }
        }

        if (min_index == NOT_FOUND)
            std::cout << "Error in Search during replace attempt\n";

        lfu_elem tmp_elem = {
            .key = key 
        };

        cache_[min_index] = tmp_elem;

        std::cout << "Id: " << min_index << " Value: " << key << "\n";
    }

    // Find element in cache_
    size_t FindElem (KeyT key){
        for (size_t i = 0; i < cache_.size(); i++){
            if (cache_[i].key == key)
                return i;
        }

        return NOT_FOUND;
    }

    // Checks if cache can't fit more elems inside
    bool IsFull (){
        return capacity_ == cache_.size();
    } 

    // Add new element to the end of 
    void PushBackElem (KeyT key){
        lfu_elem tmp_elem = {
            .key = key,
            .hit_count = 0
        };

        cache_.push_back(tmp_elem);    
    }

    void Dump (){
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
