#ifndef LFU_HPP
#define LFU_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits.h>
#include <stdint.h>
#include <map>
#include <set>
#include <unordered_map>

namespace LFU_CACHE
{

enum FIND_STATE
{
    HIT,
    NOT_HIT
};


template<typename PageT, typename KeyT>
class LFU final {
    struct LFU_ELEM;
    struct FREQ_NODE;

    using size_type = typename std::unordered_map<KeyT, FREQ_NODE>::size_type;

    struct LFU_ELEM
    {
        PageT    page;
        KeyT     key;
    };

    struct FREQ_NODE
    {
        int value;

        std::set<LFU_ELEM> items;
        FREQ_NODE* prev;
        FREQ_NODE* next;
    };
    
    size_type capacity_;
    long int  total_hit_count_ = 0;
    
    std::unordered_map<KeyT, FREQ_NODE> cache_;

public:
    LFU (size_t capacity) : capacity_(capacity) {
        cache_.reserve(capacity);
    }


    void Dump () const {
        std::cout << "------- Dump of Class: LFU -------\n";
        std::cout << "Capacity: " << capacity_     << "\n";
        std::cout << "Size: "     << cache_.size() << "\n";

        // std::cout << "Elements in cache: \n";

        // for (size_t i = 0; i < cache_.size(); i++){
        //     std::cout << i <<": Key = " << cache_[i].key << ", HitCount = " << cache_[i].hit_count << "\n";
        // }

        std::cout << "------- End of dump --------------\n";   
    }
}; 

};

#endif
