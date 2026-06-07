#pragma once
#include "model.hpp"
#include "statistics.hpp"
#include <cstdint>

namespace mc {

class ParallelSimulator {
public:
    explicit ParallelSimulator(int num_threads);

    Statistics run(const IModel& model, int64_t n_samples, unsigned seed = 42) const;

private:
    int num_threads_;
};

}
