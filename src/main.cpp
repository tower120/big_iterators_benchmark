#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/lower_bound.hpp>

template<class TimeUnit = std::chrono::milliseconds, class Closure>
static auto measure(Closure&& closure){
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    closure();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    return duration_cast<TimeUnit>(t2-t1).count();
}

template<class TimeUnit = std::chrono::milliseconds, class Closure>
static auto benchmark(unsigned int times, Closure&& closure){
    return measure<TimeUnit>([&]() {
        for (unsigned int i = 0; i < times; ++i) closure();
    });
}



int main()
{
    using namespace ranges;

    const std::size_t size  = 1'000'000;    // 4 Mb
    const std::size_t times = 1'0000'000;

    const auto forward = [](const auto& _) -> const auto& { return _; };

    std::vector<int> vec;

    int target_value;
    {
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis(0, size);

        target_value = dis(gen);

        for(std::size_t i = 0; i<size; ++i){
            vec.push_back(i);
        }
    }

    // x1
    auto list  = vec
            | view::transform(forward);

    // x8 (it's actually not that hard to get this IRL)
    auto list2 = vec
            | view::transform(forward)
            | view::transform(forward)
            | view::transform(forward)
            | view::transform(forward)
            | view::transform(forward)
            | view::transform(forward)
            | view::transform(forward)
            | view::transform(forward);

    {
        int found = 0;
        const auto t = benchmark(times, [&](){
            found += *lower_bound(list, target_value);
        });

        std::cout << found << " "  << t << std::endl;
    }
    {
        int found = 0;
        const auto t = benchmark(times, [&](){
            found += *lower_bound(list2, target_value);
        });

        std::cout << found << " "  << t << std::endl;
    }


    return 0;
}