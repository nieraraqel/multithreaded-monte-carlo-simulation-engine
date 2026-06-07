#pragma once
#include <cmath>
#include <cstdint>

namespace mc {

struct Statistics {
    double   mean;
    double   variance;
    double   std_dev;
    double   ci_low;
    double   ci_high;
    int64_t  n_samples;
    double   elapsed_ms;

    double error(double reference) const { return std::abs(mean - reference); }
};

struct WelfordAccumulator {
    int64_t n     = 0;
    double  m     = 0.0;
    double  m2    = 0.0;

    void add(double x) {
        ++n;
        double delta  = x - m;
        m            += delta / n;
        double delta2 = x - m;
        m2           += delta * delta2;
    }

    double mean()     const { return m; }
    double variance() const { return n > 1 ? m2 / (n - 1) : 0.0; }
    double std_dev()  const { return std::sqrt(variance()); }
};

inline WelfordAccumulator merge(const WelfordAccumulator& a, const WelfordAccumulator& b) {
    if (a.n == 0) return b;
    if (b.n == 0) return a;

    WelfordAccumulator result;
    result.n  = a.n + b.n;
    double delta = b.m - a.m;
    result.m  = (a.m * a.n + b.m * b.n) / result.n;
    result.m2 = a.m2 + b.m2 + delta * delta * a.n * b.n / result.n;
    return result;
}

inline Statistics make_statistics(const WelfordAccumulator& acc, double elapsed_ms) {
    double sd = acc.std_dev();
    double margin = 1.96 * sd / std::sqrt(static_cast<double>(acc.n));
    return Statistics{
        acc.mean(),
        acc.variance(),
        sd,
        acc.mean() - margin,
        acc.mean() + margin,
        acc.n,
        elapsed_ms
    };
}

}
 
