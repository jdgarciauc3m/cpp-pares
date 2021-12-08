#include <chrono>
#include <execution>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <oneapi/tbb.h>

constexpr double square(double x) { return x * x; }

double compute_pi_seq(long n) {
  const double deltax = 1.0 / static_cast<double>(n);
  double sum = 0;
  for (long i = 0; i < n; ++i) {
    const double x = (static_cast<double>(i) + 0.5) * deltax;
    sum += 1.0 / (1.0 + square(x));
  }
  return 4 * sum * deltax;
}

double compute_pi_tbb(long n) {
  const double deltax = 1.0 / static_cast<double>(n);
  double sum = tbb::parallel_reduce(
      tbb::blocked_range<long>{0, n}, 0.0,
      [=](const tbb::blocked_range<long> &range, double init) {
        for (long i = range.begin(); i != range.end(); ++i) {
          const double x = (static_cast<double>(i) + 0.5) * deltax;
          init += 1.0 / (1.0 + square(x));
        }
        return init;
      },
      [](auto x, auto y) { return x + y; });
  return 4 * sum * deltax;
}

template <typename T> class counter {
public:
  explicit counter(T n) : value_{n} {}
  T operator*() const { return value_; }
  bool operator==(const counter<T> &other) const = default;
  counter<T> &operator++() {
    ++value_;
    return *this;
  }
  counter<T> operator++(int) {
    const auto r{*this};
    ++value_;
    return r;
  }
  counter<T> &operator+=(T n) {
    value_ += n;
    return *this;
  }

private:
  T value_;
};

double compute_pi_stl_par(long n) {
  const double deltax = 1.0 / static_cast<double>(n);
  double sum = std::transform_reduce(
      std::execution::par, counter<long>{0}, counter{n}, 0.0,
      [](auto x, auto y) { return x + y; },
      [deltax](auto v) {
        const double x = (static_cast<double>(v) + 0.5) * deltax;
        return 1.0 / (1.0 + square(x));
      });
  return 4 * sum * deltax;
}

double compute_pi_tbb_deterministic(long n) {
  const double deltax = 1.0 / static_cast<double>(n);
  double sum = tbb::parallel_deterministic_reduce(
      tbb::blocked_range<long>{0, n}, 0.0,
      [=](const tbb::blocked_range<long> &range, double init) {
        for (long i = range.begin(); i != range.end(); ++i) {
          const double x = (static_cast<double>(i) + 0.5) * deltax;
          init += 1.0 / (1.0 + square(x));
        }
        return init;
      },
      [](auto x, auto y) { return x + y; });
  return 4 * sum * deltax;
}

double compute_pi_tbb_for(long n) {
  const double deltax = 1.0 / static_cast<double>(n);
  std::atomic<double> sum = 0.0;
  tbb::parallel_for(0L, n, [&](long i) {
    const double x = (static_cast<double>(i) + 0.5) * deltax;
    sum += 1.0 / (1.0 + square(x));
  });
  return 4 * sum * deltax;
}

template <typename F, typename S>
void measure(std::string_view name, F f, long n, S &score) {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  auto result = f(n);
  auto t2 = high_resolution_clock::now();
  score[std::string(name)] = duration_cast<milliseconds>(t2 - t1).count();
  std::cout << "result [" << name << "] = " << std::setprecision(10) << result
            << "\n";
}

int main() {
  constexpr long num_iterations = 100'000'000;
  std::map<std::string, unsigned long> scoreboard;
  measure("0: Sequential", compute_pi_seq, num_iterations, scoreboard);
  measure("1: TBB reduce", compute_pi_tbb, num_iterations, scoreboard);
  measure("2: TBB det_reduce", compute_pi_tbb_deterministic, num_iterations,
          scoreboard);
  measure("3: TBB for + atomic", compute_pi_tbb_for, num_iterations,
          scoreboard);
  measure("4: STL par", compute_pi_stl_par, num_iterations, scoreboard);

  double baseline = static_cast<double>(scoreboard["0: Sequential"]);
  for (auto &[name, value] : scoreboard) {
    std::cout << name << " -> " << value
              << " ms [speedup = " << baseline / static_cast<double>(value)
              << "]\n";
  }
  return 0;
}
