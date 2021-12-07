#include <chrono>
#include <iostream>
#include <oneapi/tbb.h>
#include <random>
#include <vector>

template <typename R>
std::vector<int> make_vector(int n, R & rng) {
  constexpr int min_value = 0;
  constexpr int max_value = 1000;
  std::uniform_int_distribution<int> dist{min_value, max_value};
  std::vector<int> result;
  result.reserve(n);
  std::generate_n(std::back_inserter(result), n, [&] { return dist(rng); });
  return result;
}

template <typename I>
void mysort(I first, I last) {
  std::size_t size = std::distance(first,last);
  if (size <= 1) return;
  auto mid = first + size/2;
  mysort(first,mid);
  mysort(std::next(mid), last);
  std::inplace_merge(first, mid, last);
}

template <typename I>
void myparsort(I first, I last) {
  std::size_t size = std::distance(first,last);
  if (size <= 1) return;
  auto mid = std::next(first, size/2);
  tbb::parallel_invoke(
      [&] { myparsort(first,mid); },
      [&] { myparsort(mid, last); });
  std::inplace_merge(first, mid, last);
}

void run_sequential() {
  constexpr int max = 10'000'000;
  std::mt19937_64 rng{0};// NOLINT
  auto v = make_vector(max, rng);
  auto w = make_vector(max, rng);

  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  std::sort(v.begin(), v.end());
  std::sort(w.begin(), w.end());
  auto t2 = high_resolution_clock::now();

  std::cout << "v -> " << v[0] << " , " << v[max - 1] << "\n";
  std::cout << "w -> " << w[0] << " , " << w[max - 1] << "\n";
  std::cout << duration_cast<milliseconds>(t2 - t1).count() << " ms\n";
}

void run_parallel() {
  constexpr int max = 10'000'000;
  std::mt19937_64 rng{0};// NOLINT
  auto v = make_vector(max, rng);
  auto w = make_vector(max, rng);

  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  myparsort(v.begin(), v.end());
  myparsort(w.begin(), w.end());
  auto t2 = high_resolution_clock::now();

  std::cout << "v -> " << v[0] << " , " << v[max - 1] << "\n";
  std::cout << "w -> " << w[0] << " , " << w[max - 1] << "\n";
  std::cout << duration_cast<milliseconds>(t2 - t1).count() << " ms\n";
}

int main() {
  run_sequential();
  run_parallel();
  return 0;
}
