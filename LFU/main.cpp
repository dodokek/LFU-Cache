
#include <cstddef>
#include <iostream>

#include "include/LFU.hpp"


int main()
{
    size_t cache_capactiy = 0;
    size_t input_size = 0;

    std::cin >> cache_capactiy >> input_size;

    LFU_CACHE::LFU<int, int> lfu_instance(cache_capactiy);

    for (size_t i = 0 ; i < input_size; i++)
    {
        int new_elem = 0;
        std::cin >> new_elem;

        lfu_instance.LookupAndHandle(new_elem);
        // lfu_instance.Dump();
    }

    lfu_instance.ShowHitcountInfo();
}