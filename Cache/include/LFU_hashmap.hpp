#ifndef LFU_HASHMAP
#define LFU_HASHMAP

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>

namespace LFU_CACHE
{

template<typename KeyT, typename PageT>
class LFU final {
    struct LFU_ELEM;
    struct FREQ_NODE;

    using freq_node_iter = typename std::list<FREQ_NODE>::iterator; 
public:    
    using size_type      = typename std::list<LFU_ELEM>::size_type;
    using elem_iter      = typename std::list<LFU_ELEM>::iterator;
    using hashmap_iter   = typename std::unordered_map<KeyT, elem_iter>::iterator;

private:
    struct LFU_ELEM
    {
        PageT          page_;
        KeyT           key_;
        freq_node_iter parent_freq_node_;

        LFU_ELEM (KeyT key, freq_node_iter parent_freq_node) 
            : key_(key), parent_freq_node_(parent_freq_node) {};
    };

    struct FREQ_NODE
    {
        int frequency_;

        std::list<LFU_ELEM> items_;

        FREQ_NODE (size_type frequency) : frequency_(frequency) {};
    };
    
    size_type capacity_;
    long int  total_hit_count_ = 0;
    
    std::unordered_map<KeyT, elem_iter> hashmap_;
    std::list<FREQ_NODE> cache_;

public:
    LFU (size_type capacity) : capacity_(capacity) {
        cache_.emplace_back (1);
    }

    bool LookupAndHandle(const KeyT& key) {
        hashmap_iter elem = hashmap_.find (key); 
        if (elem == hashmap_.end()){
            AddNewItem (key);
            return false;      
        } else {
            HandleExistingItem (elem);
            return true;
        }
    }

private:
    void AddNewItem (const KeyT& key) {
        freq_node_iter begin_freq_list = cache_.begin();

        auto& items_list = begin_freq_list->items_;
        
        if (IsFull()){
            auto item_to_erase = items_list.begin();
            items_list.erase (item_to_erase);
            hashmap_.erase (item_to_erase->key_);
        } 

        items_list.emplace_back (key, begin_freq_list);
        
        hashmap_[key] = std::prev(items_list.end());

    }


    void HandleExistingItem (hashmap_iter item) {
        total_hit_count_++;
        elem_iter content = item->second;

        freq_node_iter cur_freq_node = content->parent_freq_node_;
        auto next_freq_node = std::next(cur_freq_node);
        if (next_freq_node == cache_.end() || ( cur_freq_node->frequency_ + 1 != next_freq_node->frequency_ )){
            next_freq_node = cache_.emplace (next_freq_node, cur_freq_node->frequency_ + 1);
        }

        next_freq_node->items_.splice (next_freq_node->items_.end(), cur_freq_node->items_, content);
        content->parent_freq_node_ = next_freq_node;
    }

public:
    bool IsFull () const {
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

        for (auto cur = hashmap_.begin(), end = hashmap_.end(); cur != end; ++cur) {
            std::cout << "\tKey: " << cur->first << " Hits: " << cur->second->parent_freq_node_->frequency_ <<  "\n";
        }

        std::cout << "------- End of dump --------------\n";   
    }
}; 

} // namespace LFU_CACHE

#endif // #ifdef LFU_HASHMAP
