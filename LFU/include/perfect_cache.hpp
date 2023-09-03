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
            LookupAndHandle(*cur, indx);
        }

        std::cout << hitcount_ << '\n';
    }
   
   
    bool LookupAndHandle (const KeyT& key, const int indx) {
        if (key_checklist_.find(key) == key_checklist_.end()) {
            HandleNewItem (key, indx);
            key_checklist_.emplace(key);

            DeleteRecentElem(key);
            return false;
        } else {
            hitcount_++;
            DeleteRecentElem(key);
            return true;
        }   
    }

private:
    void DeleteRecentElem (const KeyT& key) {
        hashmap_[key].pop_back();
        if (hashmap_[key].empty()) {
            hashmap_.erase(key);
        }
    }

    void HandleNewItem (const KeyT& key, const size_type indx) {
        if (IsFull()) {
            if (DeleteLeastFreq (key, indx) == false)
                return;
        }
        cache_.emplace_front(key);
        hashmap_[key].push_front(indx);
    }


    bool DeleteLeastFreq (const KeyT& key, const size_type indx) {

        if (hashmap_[key].size() == 1)
            return false;

        size_type max_new_dist = 0;
        for (size_type i = indx + 1; i < input_size_; i++) {
            if (input_data_[i] == key)
                max_new_dist = i - indx;
        }

        size_type id_item_to_delete = 0;
        for (auto cur_elem = cache_.begin(), end = cache_.end(), counter = 0;
             cur_elem != end; ++cur_elem, ++counter) {

            size_type new_distance = hashmap_[cur_elem->key_].back() - indx;
            if (new_distance > max_new_dist) {
                id_item_to_delete = counter;
                max_new_dist = new_distance;
            }
        }

        auto elem_to_del = cache_.begin();
        std::advance(elem_to_del, id_item_to_delete);   
        key_checklist_.erase(elem_to_del->key_);
        cache_.erase(elem_to_del);

        return true;
    }


    bool DeleteLeastFreqNaive(const KeyT& key, const size_type indx) {
        
        size_type max_new_dist = 0;
        for (size_type i = indx + 1; i < input_size_; i++) {
            if (input_data_[i] == key)
                max_new_dist = i - indx;
        }

        // No need to insert this item
        if (max_new_dist == 0)
            return false;

        size_type id_item_to_delete = *key_checklist_.begin();
        for (size_type i = 0; i < input_size_; i++) {
            for (size_type j = i + 1; j < input_size_; j++) {
                if (input_data_[i] == input_data_[j] || j == input_size_ - 1) {
                    size_type new_distance = j - i + indx;
                    if (new_distance > max_new_dist) {
                        id_item_to_delete = i;
                        max_new_dist = new_distance;
                        // std::cout 
                    }
                }
            }
        }

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
            std::cout << "Key: " << elem.key_ << '\n';
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