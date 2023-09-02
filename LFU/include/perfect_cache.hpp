#ifndef PERFECT_HPP
#define PERFECT_HPP

#include <climits>
#include <unordered_map>
#include <list>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <iterator>


namespace PERFECT_CACHE
{

template<typename KeyT, typename PageT>
class PerfectCache final
{
    struct LFU_ELEM;

public:    
    using cache_iterator = typename std::list<LFU_ELEM>::iterator;
    using size_type      = typename std::vector<LFU_ELEM>::size_type;
    using hashmap_iter   = typename std::unordered_map<KeyT, cache_iterator>::iterator;

private:
    struct LFU_ELEM
    {
        PageT          page_;
        KeyT           key_;

        LFU_ELEM (KeyT key) 
            : key_(key) {};
    };


    std::list<LFU_ELEM> cache_; // Main cache
    std::unordered_map<KeyT, cache_iterator> hashmap_; // Hashmap to find elems in main cache
    std::unordered_set<KeyT>  help_buffer_;  // Set to check if element was already found
    std::vector<KeyT> input_data_;

    size_type input_size_;
    size_type capacity_;
    int hitcount_ = 0;

public:
    PerfectCache (size_type capacity) : capacity_(capacity) {
        input_data_.reserve(capacity);
    }
 
    void GetInput () {
        std::cin >> input_size_;

        for (size_type i = 0 ; i < input_size_; i++) {
            KeyT new_elem;
            std::cin >> new_elem;

            if (!std::cin.good())
                throw std::runtime_error{"Bad input: wrong Key"};
            
            input_data_.push_back(new_elem);
        }
    }


    void RunCache () {
        for (auto cur = input_data_.begin(), end = input_data_.end(), indx = 0;
             cur != end; ++cur, ++indx) {
            LookupAndHandle(*cur, indx);
            Dump ();
        }

        std::cout << hitcount_ << '\n';
    }
   
   
    bool LookupAndHandle (const KeyT& key, const int indx) {
        hashmap_iter elem = hashmap_.find(key);

        if (elem == hashmap_.end()) {
            HandleNewItem (key, indx);
            return false;
        } else {
            hitcount_++;
            return true;
        }
    }


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


private:
    void HandleNewItem (const KeyT& key, const size_type indx) {
        if (IsFull()) {
            DeleteLeastFreqNaive (key, indx);
            // DeleteLeastFreq ();
        }
        cache_.emplace_front(key);
        hashmap_[key] = std::next (cache_.begin());
    }


    void DeleteLeastFreq () {
        // if (input_data_.)
        
        // auto elem_to_delete = std::prev(cache_.end());

    }


    void DeleteLeastFreqNaive(const KeyT& key, const size_type indx) {
        
        size_type max_new_dist = 0;
        for (size_type i = indx + 1; i < input_data_.size(); i++) {
            if (input_data_[i] == key)
                max_new_dist = i - indx;
        }

        std::cout << "Max distance: " << max_new_dist << '\n';
        // No need to insert this item
        if (max_new_dist == 0)
            return;

        auto elem_to_delete = std::prev (cache_.end());
        for (auto cache_iter = cache_.begin(), end = cache_.end();
             cache_iter != end; ++cache_iter) {

            for (auto cur_cache_iter = std::next(cache_iter); cur_cache_iter != cache_.end(); ++cur_cache_iter) {
                if (cur_cache_iter->key_ == cache_iter->key_) {
                    size_type cur_dist = std::distance(cache_iter, cur_cache_iter);
                    if (cur_dist > max_new_dist) {
                        elem_to_delete = cache_iter;
                        max_new_dist = cur_dist;
                    }

                }
            }
        }
        
        hashmap_.erase(elem_to_delete->key_);
        cache_.erase(elem_to_delete);
    }

};

} // namespace PERFECT_CACHE

#endif // PERFECT_HPP 