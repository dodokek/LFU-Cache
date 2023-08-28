#ifndef LFU_HPP
#define LFU_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits.h>
#include <list>
#include <stdint.h>
#include <map>
#include <set>
#include <unordered_map>

namespace LFU_CACHE
{

template<typename PageT, typename KeyT>
class LFU final {
    struct LFU_ELEM;
    struct FREQ_NODE;

    using size_type      = typename std::list<LFU_ELEM>::size_type;
    using freq_node_iter = typename std::list<FREQ_NODE>::iterator; 
    using elem_iter      = typename std::list<LFU_ELEM>::iterator;
    using hashmap_iter   = typename std::unordered_map<KeyT, elem_iter>;

    struct LFU_ELEM
    {
        PageT          page;
        KeyT           key;
        freq_node_iter parent_freq_node;
    };

    struct FREQ_NODE
    {
        int frequency;

        std::list<LFU_ELEM> items;
    };
    
    size_type capacity_;
    long int  total_hit_count_ = 0;
    //TODO remove this shit
    PageT init_page_;
    
    std::unordered_map<KeyT, elem_iter> hashmap_;
    std::list<FREQ_NODE> cache_;


public:
    LFU (size_t capacity) : capacity_(capacity) {
        cache_.emplace_back(1);
    }

    bool LookupAndHandle(const KeyT& key)
    {
        auto elem = hashmap_.find(key); 
        if (elem == hashmap_.end()){
            AddNewItem(key);
            return false;      
        } else {
            HandleExistingItem(elem);
            return true;
        }
    }


    void AddNewItem (const KeyT& key)
    {
        freq_node_iter begin_freq_list = cache_.begin();

        auto& items_list = begin_freq_list.items;
        
        if (IsFull()){
            auto& item_to_erase = items_list.begin();
            items_list.erase(item_to_erase);
            hashmap_.erase (item_to_erase.key);
        } 

        items_list.emplace (init_page_, key, begin_freq_list);
        hashmap_[key] = items_list.back();
    }


    void HandleExistingItem (hashmap_iter new_item)
    {
        KeyT&     new_key     = new_item->first;   
        LFU_ELEM& new_content = new_item->second;

        freq_node_iter cur_freq_node = new_content.parent_freq_node;
    }


    bool IsFull () const 
    {
        return hashmap_.size() == capacity_;
    }



    void Dump () const {
        std::cout << "------- Dump of Class: LFU -------\n";
        std::cout << "Capacity: " << capacity_     << "\n";
        std::cout << "Size: "     << hashmap_.size() << "\n";

        // std::cout << "Elements in cache: \n";

        // for (size_t i = 0; i < cache_.size(); i++){
        //     std::cout << i <<": Key = " << cache_[i].key << ", HitCount = " << cache_[i].hit_count << "\n";
        // }

        std::cout << "------- End of dump --------------\n";   
    }
}; 

};

#endif
