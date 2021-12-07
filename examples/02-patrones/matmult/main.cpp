#include "matrix.hpp"
#include <chrono>
#include <iostream>
#include <random>

template <typename random_generator>
matrix make_matrix(int r, int c, random_generator &rng) {
  matrix m{r, c};
  std::uniform_real_distribution<double> dist{-1000, 1000};
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      m(i, j) = dist(rng);
    }
  }
  return m;
}

int main() {
  std::random_device rng;
  constexpr int max = 2000;
  matrix a = make_matrix(max, max, rng);
  matrix b = make_matrix(max, max, rng);

  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  matrix c = a * b;
  auto t2 = high_resolution_clock::now();
  std::cout << "c[0,0] = " << c(0, 0) << "\n";
  std::cout << "Time = " << duration_cast<milliseconds>(t2 - t1).count()
            << " ms\n";
}
