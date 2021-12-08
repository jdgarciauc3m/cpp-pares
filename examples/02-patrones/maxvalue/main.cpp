#include <chrono>
#include <execution>
#include <iostream>
#include <map>
#include <oneapi/tbb.h>
#include <random>
#include <vector>

template <typename Rng> auto make_vector(int n, Rng &rng) {
  std::uniform_int_distribution dist{0, 100};
  std::vector<int> v;
  v.reserve(n);
  std::generate_n(std::back_inserter(v), n, [&] { return dist(rng); });
  return v;
}

template <typename T> auto max_value(const std::vector<T> &v) {
  T result = std::numeric_limits<T>::min();
  for (std::size_t i = 0; i < v.size(); ++i) {
    result = std::max(result, v[i]);
  }
  return result;
}

template <typename T> auto max_value_rng(const std::vector<T> &v) {
  T result = std::numeric_limits<T>::min();
  for (auto x : v) {
    result = std::max(result, x);
  }
  return result;
}

template <typename T> auto max_value_stl_par(const std::vector<T> &v) {
  return std::reduce(std::execution::par, v.begin(), v.end(),
                     std::numeric_limits<T>::min(),
                     [](auto x, auto y) { return std::max(x, y); });
}

template <typename T> auto max_value_stl_parunseq(const std::vector<T> &v) {
  return std::reduce(std::execution::par_unseq, v.begin(), v.end(),
                     std::numeric_limits<T>::min(),
                     [](auto x, auto y) { return std::max(x, y); });
}

template <typename T> auto max_value_stl_unseq(const std::vector<T> &v) {
  return std::reduce(std::execution::unseq, v.begin(), v.end(),
                     std::numeric_limits<T>::min(),
                     [](auto x, auto y) { return std::max(x, y); });
}

template <typename T> auto max_value_reduce_idx(const std::vector<T> &v) {
  return tbb::parallel_reduce(
      tbb::blocked_range<std::size_t>{0, v.size()},
      std::numeric_limits<T>::min(),
      [&](const auto &rng, auto x) {
        for (std::size_t i = rng.begin(); i != rng.end(); ++i) {
          x = std::max(x, v[i]);
        }
        return x;
      },
      [](auto x, auto y) { return std::max(x, y); });
}

template <typename T> auto max_value_reduce_iter(const std::vector<T> &v) {
  return tbb::parallel_reduce(
      tbb::blocked_range{v.begin(), v.end()}, std::numeric_limits<T>::min(),
      [](const auto &rng, auto x) {
        for (auto value : rng) {
          x = std::max(x, value);
        }
        return x;
      },
      [](auto x, auto y) { return std::max(x, y); });
}

template <typename F, typename C> auto measure(F &&f, const C &cont) {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  auto result = f(cont);
  auto t2 = high_resolution_clock::now();
  auto dif = duration_cast<microseconds>(t2 - t1).count();
  std::cout << "result = " << result << "\n";
  return dif;
}

int main() {
  constexpr int max = 100'000'000;
  std::random_device rng;
  auto v = make_vector(max, rng);
  std::map<std::string, std::size_t> exec_time;

  exec_time["0: Baseline"] = measure(max_value<int>, v);
  exec_time["1: Range for"] = measure(max_value_rng<int>, v);
  exec_time["2: TBB Index"] = measure(max_value_reduce_idx<int>, v);
  exec_time["3: TBB Iterator"] = measure(max_value_reduce_iter<int>, v);
  exec_time["4: STL par"] = measure(max_value_stl_par<int>, v);
  exec_time["5: STL par unseq"] = measure(max_value_stl_parunseq<int>, v);
  exec_time["6: STL unseq"] = measure(max_value_stl_unseq<int>, v);

  auto baseline = exec_time["0: Baseline"];
  for (auto [label, time] : exec_time) {
    std::cout << label << " -> " << time << " us"
              << " [speedup = " << double(baseline) / time << "]\n";
  }

  return 0;
}
