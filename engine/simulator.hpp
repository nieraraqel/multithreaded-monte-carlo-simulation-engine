#pragma once
#include "model.hpp"
#include "statistics.hpp"
#include <cstdint>

namespace mc {

class Simulator {
public:
    Statistics run(const IModel& model, int64_t n_samples, unsigned seed = 42) const;
};

}
