#ifndef PERFECT_HPP
#define PERFECT_HPP

#include <cstddef>
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

    bool LookupAndHandle (const KeyT& key) {
        hashmap_iter elem = hashmap_.find(key);

        if (elem == hashmap_.end()) {
            HandleNewItem (key);
            return false;
        } else {
            hitcount_++;
            return true;
        }
    }


    void RunCache () {
        for (auto cur = input_data_.begin(), end = input_data_.end(); cur != end; ++cur) {
            LookupAndHandle(*cur);
            Dump ();
        }

        std::cout << "Hitcount: " << hitcount_ << '\n';
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
    void HandleNewItem (const KeyT& key) {
        if (IsFull()) {
            DeleteLeastFreq ();
        }
        cache_.emplace_front(key);
        hashmap_[key] = std::next (cache_.begin());
    }


    void DeleteLeastFreq () {
        auto elem_to_delete = std::prev(cache_.end());

        for (auto cur_elem = cache_.begin(), end = cache_.end(); cur_elem != end; ++cur_elem) {
            if (hashmap_.find (cur_elem->key_) != hashmap_.end()) {
                if (help_buffer_.find(cur_elem->key_) != help_buffer_.end())
                    elem_to_delete = cur_elem;
                help_buffer_.insert(cur_elem->key_);
            }
        }

        std::cout << "\tElem to delete: " << elem_to_delete->key_ << '\n';

        help_buffer_.erase(elem_to_delete->key_);
        hashmap_.erase(elem_to_delete->key_);
        cache_.erase(elem_to_delete);
    }

};

} // enum PERFECT_CACHE

#endif // PERFECT_HPP 