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

    void GetInput ()
    {
        
    }

};

} // enum PERFECT_CACHE

#endif // PERFECT_HPP 