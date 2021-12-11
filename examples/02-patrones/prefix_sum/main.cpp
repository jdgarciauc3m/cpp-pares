#include <iostream>
#include <iterator>
#include <oneapi/tbb.h>
#include <random>

template <typename generator> auto make_vector(std::size_t n, generator &rng) {
  std::uniform_int_distribution<int> gen{1, 1000};
  std::vector<int> result;
  result.reserve(n);
  std::generate_n(std::back_inserter(result), n, [&]() { return gen(rng); });
  return result;
}

auto prefix_sum(const std::vector<int> &v) {
  const std::size_t max = v.size();
  std::vector<int> result;
  result.reserve(max);
  result.push_back(v[0]);
  for (std::size_t i = 1; i < max; ++i) {
    result.push_back(result[i - 1] + v[i]);
  }
  return result;
}

auto prefix_sum_tbb(const std::vector<int> &v) {
  const std::size_t max = v.size();
  std::vector<int> result(max);
  tbb::parallel_scan(
      tbb::blocked_range<std::size_t>{1, max}, int{0},
      [v, &result](const tbb::blocked_range<std::size_t> &rng, int sum,
                   bool is_final) {
        for (auto i = rng.begin(); i != rng.end(); ++i) {
          sum += v[i];
          if (is_final) {
            result[i] = sum;
          }
        }
        return sum;
      },
      [](auto x, auto y) { return x + y; });
  return result;
}

int main() {
  constexpr int max = 1'000'000;
  std::random_device rng;
  auto v = make_vector(max, rng);
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  auto r = prefix_sum(v);
  auto t2 = high_resolution_clock::now();
  auto t = prefix_sum(v);
  auto t3 = high_resolution_clock::now();

  std::cout << "Sequential: " << duration_cast<microseconds>(t2 - t1).count()
            << "ms\n";
  std::cout << "Parallel scan: " << duration_cast<microseconds>(t3 - t2).count()
            << "ms\n";

  std::copy(begin(v), begin(v) + 5, std::ostream_iterator<int>(std::cout, " "));
  std::cout << "\nr = ";
  std::copy(begin(r), begin(r) + 5, std::ostream_iterator<int>(std::cout, " "));
  std::cout << "\nt = ";
  std::copy(begin(t), begin(t) + 5, std::ostream_iterator<int>(std::cout, " "));
  return 0;
}
