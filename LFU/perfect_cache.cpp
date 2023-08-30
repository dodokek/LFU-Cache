#include "perfect_cache.hpp"

int main()
{
    size_t cache_capactiy;
    
    std::cin >> cache_capactiy;
    if (!std::cin.good())
        throw std::runtime_error{"Bad input: cache capacity"};

    PERFECT_CACHE::PerfectCache<int, int> cache_instance(cache_capactiy);

    cache_instance.GetInput();
    cache_instance.RunCache();
}
