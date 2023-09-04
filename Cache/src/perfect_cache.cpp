#include "perfect_cache.hpp"
#include <vector>

int main()
{
    size_t cache_capactiy;
    size_t input_size;
    std::vector<int> input_data;

    std::cin >> cache_capactiy;
    if (!std::cin.good())
        throw std::runtime_error{"Bad input: cache capacity"};

    PERFECT_CACHE::GetInput<int>(std::cin, input_size, input_data);

    PERFECT_CACHE::PerfectCache<int, int> cache_instance(cache_capactiy, input_size, input_data);
    cache_instance.RunCache();
}
