#ifndef AVG_TBBSTL_HPP
#define AVG_TBBSTL_HPP

#include <oneapi/tbb.h>

void square_tbbstl(std::vector<int> &v) {
  using namespace oneapi::tbb;
  using range = blocked_range<std::vector<int>::iterator>;
  parallel_for(range{v.begin(), v.end()},
               [](const range &r) {
                 std::transform(r.begin(), r.end(), r.begin(),
                                [](auto x) { return x * x; });
               });
}

double average_tbbstl(const std::vector<int> &v) {
  using namespace oneapi::tbb;
  using range_type = blocked_range<std::vector<int>::const_iterator>;
  double sum = parallel_reduce(
      range_type{v.begin(), v.end()}, 0.0,
      [](const range_type &r, double x) -> double {
        return std::reduce(r.begin(), r.end(), x);
      },
      std::plus<double>{});
  return sum / static_cast<double>(v.size());
}

#endif//AVG_TBBSTL_HPP
