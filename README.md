# Multithreaded Monte Carlo Simulation Engine

Developed as part of a study of Monte Carlo methods and parallel computing.

A C++17 Monte Carlo simulation engine for creating custom probabilistic models, running parallel simulations, and collecting statistical and performance benchmarks.

## Features

* Single-threaded and multithreaded Monte Carlo simulations
* Simple interface for implementing custom probabilistic models
* Statistical analysis of simulation results
* Confidence interval estimation
* Performance benchmarking
* Configurable number of worker threads
* C++17 implementation with minimal dependencies

## Implemented Examples

### Random Points Method

Estimation of π using the ratio between the area of a unit circle and its bounding square.

### Coprime Numbers Method

Estimation of π using the probability that two randomly selected integers are coprime:

[
P(\gcd(a,b)=1)=\frac{6}{\pi^2}
]

### Buffon's Needle Method

Estimation of π using the probability of a randomly dropped needle crossing parallel lines.

## Project Structure

```text id="t7kq21"
.
├── engine/
│   ├── model.hpp
│   ├── simulator.hpp
│   ├── simulator.cpp
│   ├── parallel_simulator.hpp
│   └── parallel_simulator.cpp
│
├── models/
│   ├── random_points_pi_model.hpp
│   ├── coprime_pi_model.hpp
│   └── buffon_needle_pi_model.hpp
│
├── main.cpp
├── Makefile
└── LICENSE
```

## Building

Requirements:

* C++17 compatible compiler
* GNU Make

Build:

```bash id="b3k9xa"
make
```

Run:

```bash id="m2p7qf"
./montecarlo
```

Clean build artifacts:

```bash id="x8v1ld"
make clean
```

## Creating Custom Models

To create a new Monte Carlo model, inherit from `mc::IModel` and implement the required methods:

```cpp id="c9p2we"
class MyModel : public mc::IModel {
public:
    double sample(std::mt19937& rng) const override {
        // Perform one experiment
        return 0.0;
    }

    double estimate(double mean) const override {
        return mean;
    }

    double reference() const override {
        return 0.0;
    }

    const char* name() const override {
        return "My Model";
    }
};
```

Then pass an instance of the model to either:

```cpp id="k1m8zd"
mc::Simulator
```

or

```cpp id="v6n3qp"
mc::ParallelSimulator
```

to execute the simulation.

## Benchmark Output

The engine reports:

* Estimated value
* Absolute error
* Execution time
* Confidence interval
* Parallel speedup

allowing comparison between different Monte Carlo methods and execution configurations.

## License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0).
