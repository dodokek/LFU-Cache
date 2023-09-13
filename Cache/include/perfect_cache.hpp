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
#define FAST_VERSION

#define DBG
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
    struct CACHE_ELEM;

public:    
    using cache_iterator = typename std::list<CACHE_ELEM>::iterator;
    using size_type      = typename std::vector<CACHE_ELEM>::size_type;
    using hashmap_iter   = typename std::unordered_map<KeyT, std::deque<KeyT>>::iterator;

private:
    struct CACHE_ELEM
    {
        PageT          page_;
        KeyT           key_;

        CACHE_ELEM (KeyT key) 
            : key_(key) {};
    };


    std::list<CACHE_ELEM> cache_; // Main cache
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
            hashmap_[*cur].push_back(indx);
        }

        for (auto cur = input_data_.begin(), end = input_data_.end(); cur != end; ++cur) {
            //if element not in cache
            if (key_checklist_.find(*cur) == key_checklist_.end()) {
                //if elem is the only one -> don't insert it in the cache
                if (hashmap_[*cur].size() == 1) {
                    continue;
                }
                // remember new cache element
                key_checklist_.insert(*cur);
                if (IsFull()) {
                    auto replace_iter = FindFurthest();
                    
                    key_checklist_.erase(replace_iter->key_);
                    *replace_iter = {*cur};

                } else {
                    cache_.emplace_back(*cur);
                }
            } else {
                ++hitcount_;
            }
            //after pushing elem to cache we don't need it in hash_table
            RemoveUsedElem(*cur);
    }

        std::cout << hitcount_ << '\n';
    }

   
private:
    cache_iterator FindFurthest() {
        size_type max_distance = 0;
        cache_iterator replace_iter{};
        for (auto cache_iter = cache_.begin(); cache_iter != cache_.end(); ++cache_iter) {
            //if value with such key was not found in buffer --> replace it
            if (hashmap_.find(cache_iter->key_) != hashmap_.end()) {
                auto deque_iter = hashmap_[cache_iter->key_].begin();
                if (*deque_iter > max_distance) {
                    max_distance = *deque_iter;
                    replace_iter = cache_iter;
                }
            } else {
                replace_iter = cache_iter;
                break;
            }
        }
        return replace_iter;
    }


    void RemoveUsedElem (KeyT& key) {
        hashmap_[key].pop_front();
        if (hashmap_[key].empty()) {
            hashmap_.erase(key);
        }
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