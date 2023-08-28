#ifndef LFU_HPP
#define LFU_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
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
    using hashmap_iter   = typename std::unordered_map<KeyT, elem_iter>::iterator;

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


    void AddNewItem (const KeyT& key)
    {
        freq_node_iter begin_freq_list = cache_.begin();

        auto& items_list = begin_freq_list->items;
        
        if (IsFull()){
            auto item_to_erase = items_list.begin();
            items_list.erase (item_to_erase);
            hashmap_.erase (item_to_erase->key);
        } 

        items_list.emplace (items_list.end() ,init_page_, key, begin_freq_list);
        
        //TODO Why it won't compile with items_list.back() ???
        hashmap_[key] = std::prev(items_list.end());

    }


    void HandleExistingItem (hashmap_iter item)
    {
        auto content = item->second;

        freq_node_iter cur_freq_node = content->parent_freq_node;
        freq_node_iter next_freq_node = std::next(cur_freq_node);

        if (next_freq_node == cache_.end() || ( cur_freq_node->frequency + 1 != next_freq_node->frequency )){
            next_freq_node = cache_.emplace (next_freq_node, cur_freq_node->frequency + 1);
        }

        next_freq_node->items.splice (next_freq_node->items.end(), cur_freq_node->items, content);
        content->parent_freq_node = next_freq_node;
    }

public:
    LFU (size_type capacity) : capacity_(capacity) {
        
        //TODO Sort of cringe
        auto first_elem = cache_.emplace (cache_.end());
        first_elem->frequency = 1;
    }

    bool LookupAndHandle(const KeyT& key)
    {
        hashmap_iter elem = hashmap_.find (key); 
        if (elem == hashmap_.end()){
            AddNewItem (key);
            return false;      
        } else {
            HandleExistingItem (elem);
            return true;
        }
    }


    bool IsFull () const 
    {
        return hashmap_.size() == capacity_;
    }


    long int GetHitcountInfo () const {
        return total_hit_count_;
    }


    void Dump () const {
        std::cout << "------- Dump of Class: LFU -------\n";
        std::cout << "Capacity: " << capacity_     << "\n";
        std::cout << "Size: "     << hashmap_.size() << "\n";

        std::cout << "Elements in cache: \n";

        for (auto cur = hashmap_.begin(), end = hashmap_.end(); cur != end; std::next(cur))
        {
            // std::cout << "Key: " << cur->first << " Value: " << cur->second << "\n";
        }

        std::cout << "------- End of dump --------------\n";   
    }
}; 

};

#endif
