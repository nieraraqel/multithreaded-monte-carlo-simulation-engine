#include "simulator.hpp"
#include "benchmark.hpp"
#include <random>

namespace mc {

Statistics Simulator::run(const IModel& model, int64_t n_samples, unsigned seed) const {
    WelfordAccumulator acc;
    std::mt19937 rng(seed);

    double elapsed = time_ms([&] {
        for (int64_t i = 0; i < n_samples; ++i) {
            acc.add(model.sample(rng));
        }
    });

    return make_statistics(acc, elapsed);
}

}
