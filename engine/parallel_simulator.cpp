#include "parallel_simulator.hpp"
#include "benchmark.hpp"
#include <thread>
#include <vector>
#include <random>

namespace mc {

ParallelSimulator::ParallelSimulator(int num_threads)
    : num_threads_(num_threads) {}

Statistics ParallelSimulator::run(const IModel& model, int64_t n_samples, unsigned seed) const {
    std::vector<WelfordAccumulator> results(num_threads_);

    int64_t base  = n_samples / num_threads_;
    int64_t extra = n_samples % num_threads_;

    double elapsed = time_ms([&] {
        std::vector<std::thread> threads;
        threads.reserve(num_threads_);

        for (int t = 0; t < num_threads_; ++t) {
            int64_t count = base + (t == num_threads_ - 1 ? extra : 0);
            unsigned thread_seed = seed + static_cast<unsigned>(t);

            threads.emplace_back([&model, &results, t, count, thread_seed] {
                std::mt19937 rng(thread_seed);
                WelfordAccumulator& acc = results[t];
                for (int64_t i = 0; i < count; ++i) {
                    acc.add(model.sample(rng));
                }
            });
        }

        for (auto& th : threads) th.join();
    });

    WelfordAccumulator combined = results[0];
    for (int t = 1; t < num_threads_; ++t) {
        combined = merge(combined, results[t]);
    }

    return make_statistics(combined, elapsed);
}

}
