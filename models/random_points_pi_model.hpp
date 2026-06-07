#pragma once
#include "../engine/model.hpp"
#include <random>
#include <cmath>

namespace mc::models {

class RandomPointsPiModel : public IModel {
public:
    double sample(std::mt19937& rng) const override {
        double x = dist_(rng);
        double y = dist_(rng);
        return (x * x + y * y <= 1.0) ? 4.0 : 0.0;
    }

    double estimate(double mean) const override {
        return mean;
    }

    double reference() const override {
        return M_PI;
    }

    const char* name() const override {
        return "Random Points";
    }

private:
    mutable std::uniform_real_distribution<double> dist_{0.0, 1.0};

};

}
