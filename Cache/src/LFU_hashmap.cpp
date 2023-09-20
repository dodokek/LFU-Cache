#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "LFU_hashmap.hpp"

int main()
{
    size_t cache_capactiy;
    size_t input_size;
    
    std::cin >> cache_capactiy >> input_size;
    if (!std::cin.good())
        throw std::runtime_error{"Bad input: cache capacity"};

    LFU_CACHE_FAST::LFU<int, int> lfu_instance(cache_capactiy);

    for (size_t i = 0 ; i < input_size; i++)
    {
        int new_elem;
        std::cin >> new_elem;

        if (!std::cin.good())
            throw std::runtime_error{"Bad input: wrong Key"};

        lfu_instance.LookupAndHandle(new_elem);
        // lfu_instance.Dump();

    }

    std::cout << lfu_instance.GetHitcountInfo() << "\n";
}
