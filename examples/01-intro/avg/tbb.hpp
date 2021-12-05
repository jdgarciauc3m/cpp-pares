#ifndef AVG_TBB_HPP
#define AVG_TBB_HPP

#include <oneapi/tbb.h>

void square_tbb(std::vector<int> &v) {
  using namespace oneapi::tbb;
  using range = blocked_range<std::vector<int>::iterator>;
  parallel_for(range{v.begin(), v.end()},
               [](const range &r) {
                 for (auto & x : r) {
                   x *= x;
                 }
               });
}

double average_tbb(const std::vector<int> &v) {
  using namespace oneapi::tbb;
  using range_type = blocked_range<std::vector<int>::const_iterator>;
  double sum = parallel_reduce(
      range_type{v.begin(), v.end()}, 0.0,
      [](const range_type &r, double x) -> double {
        for (double value : r) {
          x += value;
        }
        return x;
      },
      [](auto x, auto y) { return x + y; });
  return sum / static_cast<double>(v.size());
}


#endif//AVG_TBB_HPP
