#pragma once
#include <chrono>
#include <functional>
#include <algorithm>
#include <vector>
#include <numeric>

namespace mc {

struct BenchmarkResult {
    double mean_ms;
    double min_ms;
    double max_ms;
};

template<typename Fn>
BenchmarkResult measure(Fn&& fn, int repeats = 5) {
    using clock = std::chrono::high_resolution_clock;

    fn();

    std::vector<double> times;
    times.reserve(repeats);

    for (int i = 0; i < repeats; ++i) {
        auto t0 = clock::now();
        fn();
        auto t1 = clock::now();
        times.push_back(
            std::chrono::duration<double, std::milli>(t1 - t0).count()
        );
    }

    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    return BenchmarkResult{
        sum / repeats,
        *std::min_element(times.begin(), times.end()),
        *std::max_element(times.begin(), times.end())
    };
}

template<typename Fn>
double time_ms(Fn&& fn) {
    using clock = std::chrono::high_resolution_clock;
    auto t0 = clock::now();
    fn();
    auto t1 = clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

}
