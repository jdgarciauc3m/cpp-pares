#ifndef AVG_STL_PAR_HPP
#define AVG_STL_PAR_HPP

#include <algorithm>
#include <execution>
#include <numeric>
#include <vector>

void square_stlpar(std::vector<int> &v) {
  std::transform(std::execution::par,
                 v.begin(), v.end(), v.begin(),
                 [](auto x) { return x * x; });
}

void square_stlunseq(std::vector<int> &v) {
  std::transform(std::execution::unseq,
                 v.begin(), v.end(), v.begin(),
                 [](auto x) { return x * x; });
}

void square_stlparunseq(std::vector<int> &v) {
  std::transform(std::execution::par_unseq,
                 v.begin(), v.end(), v.begin(),
                 [](auto x) { return x * x; });
}

double average_stlpar(const std::vector<int> &v) {
  double s = std::reduce(std::execution::par, v.begin(), v.end(), 0.0,
                         [](auto x, auto y) { return x + y; });
  return s / static_cast<double>(v.size());
}

double average_stlunseq(const std::vector<int> &v) {
  double s = std::reduce(std::execution::unseq, v.begin(), v.end(), 0.0,
                         [](auto x, auto y) { return x + y; });
  return s / static_cast<double>(v.size());
}

double average_stlparunseq(const std::vector<int> &v) {
  double s = std::reduce(std::execution::par_unseq, v.begin(), v.end(), 0.0,
                         [](auto x, auto y) { return x + y; });
  return s / static_cast<double>(v.size());
}

#endif//AVG_STL_PAR_HPP
