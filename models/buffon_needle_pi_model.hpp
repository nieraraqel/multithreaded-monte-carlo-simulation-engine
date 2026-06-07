#pragma once
#include "../engine/model.hpp"
#include <random>
#include <cmath>

namespace mc::models {

class BuffonNeedlePiModel : public IModel {
public:
    double sample(std::mt19937& rng) const override {
        double y = distY_(rng);
        double theta = distTheta_(rng);

        double projection = 0.5 * std::sin(theta);

        return (y <= projection) ? 1.0 : 0.0;
    }

    double estimate(double mean) const override {
        return 2.0 / mean;
    }

    double reference() const override {
        return M_PI;
    }

    const char* name() const override {
        return "Buffon Needle";
    }

private:
    mutable std::uniform_real_distribution<double> distY_{0.0, 0.5};
    mutable std::uniform_real_distribution<double> distTheta_{0.0, M_PI};
};

}