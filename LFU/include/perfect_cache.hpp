#ifndef PERFECT_HPP
#define PERFECT_HPP

#include <climits>
#include <cstddef>
#include <deque>
#include <unordered_map>
#include <list>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <iterator>
#include <typeinfo>
#include <assert.h>
#include <type_traits>


namespace PERFECT_CACHE
{
// #define FAST_VERSION

// #define DBG
#ifdef DBG
    #define dbg_cout(X) std::cout X;
    #define DBG__(X) X
#else
    #define dbg_cout(X) ;
    #define DBG__(X) ;
#endif

template<typename KeyT, typename PageT>
class PerfectCache final
{
    struct LFU_ELEM;

public:    
    using cache_iterator = typename std::list<LFU_ELEM>::iterator;
    using size_type      = typename std::vector<LFU_ELEM>::size_type;
    using hashmap_iter   = typename std::unordered_map<KeyT, std::deque<KeyT>>::iterator;

private:
    struct LFU_ELEM
    {
        PageT          page_;
        KeyT           key_;

        LFU_ELEM (KeyT key) 
            : key_(key) {};
    };


    std::list<LFU_ELEM> cache_; // Main cache
    std::unordered_map<KeyT, std::deque<size_type>> hashmap_; // Hashmap to find elems in main cache
    std::unordered_set<KeyT> key_checklist_;

    size_type capacity_;
    size_type input_size_;
    int hitcount_ = 0;

    std::vector<KeyT>& input_data_;

public:
    PerfectCache (size_type capacity, size_type input_size, std::vector<KeyT>& input_data) 
    : capacity_(capacity), input_size_(input_size), input_data_(input_data) {
        input_data_.reserve(capacity);
    }

    void RunCache () {
        for (auto cur = input_data_.begin(), end = input_data_.end(), indx = 0;
             cur != end; ++cur, ++indx) {
            hashmap_[*cur].push_front(indx);
        }

        
        for (auto cur = input_data_.begin(), end = input_data_.end(), indx = 0;
             cur != end; ++cur, ++indx) {
            dbg_cout( << "==Incoming key: " << *cur << '\n');
            DeleteRecentElem(*cur);
            LookupAndHandle(*cur, indx);
            DBG__(Dump());
        }

        std::cout << hitcount_ << '\n';
    }
   
   
private:
    void DeleteRecentElem (const KeyT& key) {
        dbg_cout( << "+++Deleting item " << hashmap_[key].back() << " Key: " << key  << '\n');
        hashmap_[key].pop_back();
    
        if (hashmap_[key].empty()) {
            hashmap_.erase(key);
        }
    }


    bool LookupAndHandle (const KeyT& key, const int indx) {
        if (key_checklist_.find(key) == key_checklist_.end()) {
            HandleNewItem (key, indx);
            return false;
        } else {
            hitcount_++;
            return true;
        }   
    }

   
    void HandleNewItem (const KeyT& key, const size_type indx) {

        if (IsFull()) {
            #ifdef FAST_VERSION
            if (DeleteLeastFreq (key, indx) == false)
                return;
            #else
            if (DeleteLeastFreqNaive (key, indx) == false)
                return;
            #endif
        }

        key_checklist_.emplace(key);
        cache_.emplace_front(key);
    }


    bool DeleteLeastFreq (const KeyT& key, const size_type indx) {

        dbg_cout( << "Amount of same elems ahead: " << hashmap_[key].size() << '\n');
        if (hashmap_[key].size() == 0) {
            dbg_cout( << "Incoming item is unique\n");
            return false;
        }

        size_type max_new_dist = 0;
        for (size_type i = indx + 1; i < input_size_; i++) {
            if (input_data_[i] == key) {
                max_new_dist = i;
                dbg_cout( << "Equal with indx: " << i << " Value: " << key << '\n');
                break;
            }
        }
        auto new_dist_save = max_new_dist;
        dbg_cout(<< "Max dist: " << max_new_dist << '\n');

        auto item_to_delete = cache_.begin();
        for (auto cur_elem = cache_.begin(), end = cache_.end();
             cur_elem != end; ++cur_elem) {

            if (hashmap_[cur_elem->key_].size() == 0) {
                item_to_delete = cur_elem;
                dbg_cout( << "Deleting unique item\n");
                break;
            }

            if (hashmap_[cur_elem->key_].back() > max_new_dist) {
                max_new_dist = hashmap_[cur_elem->key_].back();
                item_to_delete = cur_elem;
                dbg_cout( << "Max new distance: " << max_new_dist << '\n');
            }
        }

        if (new_dist_save == max_new_dist)
            return false;

        key_checklist_.erase(item_to_delete->key_);
        cache_.erase(item_to_delete);

        return true;
    }


    bool DeleteLeastFreqNaive(const KeyT& key, const size_type indx) {
        
        size_type max_new_dist = 0;
        for (size_type i = indx + 1; i < input_size_; i++) {
            if (input_data_[i] == key) {
                max_new_dist = i;
                dbg_cout( << "Equal with indx: " << i << " Value: " << key << '\n');
                break;
            }
        }

        auto new_dist_save = max_new_dist;
        dbg_cout( << "\tMax new dist: " << max_new_dist << '\n');
        // No need to insert this item
        if (max_new_dist == 0)
            return false;

        size_type id_item_to_delete = *key_checklist_.begin();
        for (auto cache_iter = cache_.begin(), end = cache_.end(); cache_iter != end; ++cache_iter) {
            for (size_type i = indx + 1; i < input_size_; i++) {
                if (input_data_[i] == cache_iter->key_) {
                    size_type new_distance = i;
                    dbg_cout( << "Indx: " << i << " Key: " << cache_iter->key_ << " Distance: " << new_distance << '\n');
                    if (new_distance > max_new_dist) {
                        id_item_to_delete = i;
                        max_new_dist = new_distance;
                    }
                    break;
                }
            }
        }

        if (max_new_dist == new_dist_save) 
            return false;

        auto elem_to_del = cache_.begin();
        std::advance(elem_to_del, id_item_to_delete);        
        key_checklist_.erase(elem_to_del->key_);
        cache_.erase(elem_to_del);

        return true;
    }

public:
    void Dump () const {

        std::cout << "------- Dump of Class: LFU -------\n";
        std::cout << "Capacity: " << capacity_     << "\n";
        std::cout << "Size: "     << cache_.size() << "\n";

        std::cout << "Elements in cache: \n";

        for (auto elem : cache_) {
            // std::cout << "Key: " << elem.key_ << '\n';
        }

        std::cout << "------- End of dump --------------\n";   
    }


    bool IsFull () const {
        return cache_.size() == capacity_;
    }

}; // End class Perfect Cache

template <typename KeyT>
void GetInput (std::istream& istream, size_t& input_size, std::vector<int>& input_data) {
    
        istream >> input_size;
        if (!istream.good())
                throw std::runtime_error{"Bad input: wrong input size"};

        for (size_t i = 0 ; i < input_size; i++) {
            KeyT new_elem;
            istream >> new_elem;

            if (!istream.good())
                throw std::runtime_error{"Bad input: wrong Key"};
            
            input_data.push_back(new_elem);
        }
    }

} // namespace PERFECT_CACHE

#endif // PERFECT_HPP 