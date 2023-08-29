#ifndef PERFECT_HPP
#define PERFECT_HPP

#include <cstddef>
#include <unordered_map>
#include <list>
#include <array>
#include <iostream>
#include <iterator>


namespace PERFECT_CACHE
{

template<typename KeyT, typename PageT, std::size_t InputMaxSize>
class PerfectCache final
{
    struct LFU_ELEM;

public:    
    using cache_iterator = typename std::list<LFU_ELEM>::iterator;
    using size_type      = typename std::array<LFU_ELEM, InputMaxSize>::size_type;

private:
    struct LFU_ELEM
    {
        PageT          page_;
        KeyT           key_;

        LFU_ELEM (KeyT key) 
            : key_(key) {};
    };


    std::list<LFU_ELEM> cache_;
    std::unordered_map<KeyT, cache_iterator> hashmap_;
    std::array<KeyT, InputMaxSize> input_data_;

    size_type input_size_;
    size_type capacity_;

public:
    PerfectCache (size_type capacity) : capacity_(capacity) {}
 
    void GetInput () {
        std::cin >> input_size_;

        for (size_type i = 0 ; i < input_size_; i++)
        {
            KeyT new_elem;
            std::cin >> new_elem;

            if (!std::cin.good())
                throw std::runtime_error{"Bad input: wrong Key"};
            
            input_data_[i] = new_elem;
        }

    }

};

} // enum PERFECT_CACHE

#endif // PERFECT_HPP 