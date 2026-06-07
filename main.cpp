#include "engine/simulator.hpp"
#include "engine/parallel_simulator.hpp"
#include "engine/benchmark.hpp"
#include "models/random_points_pi_model.hpp"
#include "models/coprime_pi_model.hpp"
#include "models/buffon_needle_pi_model.hpp"

#include <cmath>
#include <cstdio>
#include <thread>
#include <vector>

static void print_separator(char c = '-', int width = 90) {
    for (int i = 0; i < width; ++i) {
        std::putchar(c);
    }
    std::putchar('\n');
}

// --- single thread -----------------------------------------------------------

static void benchmark_single(const mc::IModel& model) {
    mc::Simulator sim;

    std::printf("\n=== Single-Thread Benchmark (%s) ===\n", model.name());

    print_separator();
    std::printf("  %-14s  %-12s  %-12s  %-10s  %s\n",
                "N (samples)", "π estimate", " error", " time (ms)", " 95% CI");
    print_separator();

    for (int64_t n : {1'000'000LL, 10'000'000LL, 100'000'000LL}) {

        mc::Statistics s = sim.run(model, n);

        double pi_est = model.estimate(s.mean);
        double error = std::abs(pi_est - model.reference());

        std::printf(
            "  %-14lld  %-12.7f  %-12.7f  %-10.1f  [%.7f, %.7f]\n",
            (long long)n,
            pi_est,
            error,
            s.elapsed_ms,
            s.ci_low,
            s.ci_high
        );
    }

    print_separator();
}

// --- multithread -------------------------------------------------------------

static void benchmark_parallel(const mc::IModel& model) {

    constexpr int64_t N = 100'000'000LL;

    mc::Simulator single_sim;
    mc::Statistics base = single_sim.run(model, N);

    double base_time = base.elapsed_ms;

    std::vector<int> thread_counts = {1, 2, 4, 8, 16};

    int hw = static_cast<int>(std::thread::hardware_concurrency());

    if (hw > 1 && hw != 16)
        thread_counts.push_back(hw);

    std::printf(
        "\n=== Parallel Benchmark (N=100M, %s) ===\n",
        model.name()
    );

    print_separator();

    std::printf(
        "  %-9s  %-12s  %-12s  %-10s  %-9s  %s\n",
        "Threads",
        "π estimate",
        " error",
        " time (ms)",
        " speedup",
        "95% CI"
    );

    print_separator();

    for (int t : thread_counts) {

        mc::ParallelSimulator psim(t);
        mc::Statistics s = psim.run(model, N);

        double pi_est = model.estimate(s.mean);

        double error =
            std::abs(pi_est - model.reference());

        double speedup =
            base_time / s.elapsed_ms;

        std::printf(
            "  %-9d  %-12.7f  %-12.7f  %-10.1f  %-9.2fx  [%.7f, %.7f]\n",
            t,
            pi_est,
            error,
            s.elapsed_ms,
            speedup,
            s.ci_low,
            s.ci_high
        );
    }

    print_separator();

    std::printf(
        "  Hardware concurrency: %d threads\n",
        hw
    );
}

int main() {

    mc::models::RandomPointsPiModel random_points;
    mc::models::CoprimePiModel coprime;
    mc::models::BuffonNeedlePiModel buffon;

    std::printf(
        "\nMonte-Carlo Engine — π estimation demo\n"
    );

    std::printf(
        "Reference value: π = %.10f\n",
        M_PI
    );

    benchmark_single(random_points);
    benchmark_parallel(random_points);

    benchmark_single(coprime);
    benchmark_parallel(coprime);

    benchmark_single(buffon);
    benchmark_parallel(buffon);

    return 0;
}