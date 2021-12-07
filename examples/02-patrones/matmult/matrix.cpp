#include "matrix.hpp"
#include <iostream>
#include <mincontracts/mincontracts.hpp>
#ifdef PARALLELFOR
#include <oneapi/tbb.h>
#endif

matrix matrix::operator*(const matrix &m) const {
  CONTRACT_PRE(columns_ == m.rows_);
  matrix result{rows_, m.columns()};
#ifdef SEQUENTIAL
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < m.columns(); ++j) {
      double sum = 0;
      for (int k = 0; k < columns_; ++k) {
        sum += this->operator()(i, j) * m(k, j);
      }
      result(i, j) = sum;
    }
  }
#else
  tbb::parallel_for(0, rows_, [*this, m, &result](int i) {
    for (int j = 0; j < m.columns(); ++j) {
      double sum = 0;
      for (int k = 0; k < columns_; ++k) {
        sum += this->operator()(i, j) * m(k, j);
      }
      result(i, j) = sum;
    }
  });
#endif
  return result;
}

std::ostream &operator<<(std::ostream &os, const matrix &m) {
  for (int i = 0; i < m.rows(); ++i) {
    for (int j = 0; j < m.columns(); ++j) {
      os << m(i, j) << " ";
    }
    os << "\n";
  }
  return os;
}
