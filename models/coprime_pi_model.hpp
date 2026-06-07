#pragma once
#include "../engine/model.hpp"
#include <random>
#include <numeric>
#include <cmath>

namespace mc::models {

class CoprimePiModel : public IModel {
public:
    double sample(std::mt19937& rng) const override {
        int a = dist_(rng);
        int b = dist_(rng);

        return (std::gcd(a, b) == 1) ? 1.0 : 0.0;
    }

    double estimate(double mean) const override {
        return std::sqrt(6.0 / mean);
    }

    double reference() const override {
        return M_PI;
    }

    const char* name() const override {
        return "Coprime Numbers";
    }

private:
    mutable std::uniform_int_distribution<int> dist_{1, 1000000};
};

}