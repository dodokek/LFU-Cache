#include <cstddef>
#include <iostream>

#include "LFU.hpp"
#include "include/LFU.hpp"

int main()
{
    size_t cache_capactiy = 0;
    size_t input_size = 0;

    std::cin >> cache_capactiy >> input_size;
    
DBG (if (!std::cin.good())
        std::cerr << "Bad input\n";
    ) 

    LFU_CACHE::LFU<int, int> lfu_instance(cache_capactiy);

    for (size_t i = 0 ; i < input_size; i++)
    {
        int new_elem = 0;
        std::cin >> new_elem;

    DBG (
        if (!std::cin.good())
            std::cerr << "Bad input\n";
        )

        lfu_instance.LookupAndHandle(new_elem);
        // lfu_instance.Dump();
    }

    std::cout << "Total hits: " << lfu_instance.GetHitcountInfo() << "\n";
}