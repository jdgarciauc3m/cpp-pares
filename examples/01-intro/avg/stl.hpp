#ifndef AVG_STL_HPP
#define AVG_STL_HPP

#include <algorithm>
#include <numeric>
#include <vector>

void square_stl(std::vector<int> &v) {
  std::transform(v.begin(), v.end(), v.begin(),
                 [](auto x) { return x * x; });
}

double average_stl(const std::vector<int> &v) {
  double s = std::reduce(v.begin(), v.end(), 0.0,
                         [](auto x, auto y) { return x + y; });
  return s / static_cast<double>(v.size());
}

#endif//AVG_STL_HPP
