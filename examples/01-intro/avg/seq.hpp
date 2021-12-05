#ifndef AVG_SEQ_HPP
#define AVG_SEQ_HPP

#include <vector>

inline void square_seq(std::vector<int> &v) {
  for (auto &x : v) {
    x *= x;
  }
}

inline double average_seq(const std::vector<int> &v) {
  double s = 0.0;
  for (auto x : v) {
    s += x;
  }
  return s / static_cast<double>(v.size());
}

#endif//AVG_SEQ_HPP
