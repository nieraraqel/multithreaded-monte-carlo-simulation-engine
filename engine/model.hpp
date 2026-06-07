#pragma once
#include <random>

namespace mc {

class IModel {
public:
    virtual ~IModel() = default;

    virtual double sample(std::mt19937& rng) const = 0;

    virtual double estimate(double mean) const {
        return mean;
    }

    virtual double reference() const = 0;

    virtual const char* name() const = 0;
};
}
