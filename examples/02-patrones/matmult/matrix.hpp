#ifndef MATMULT_MATRIX_HPP
#define MATMULT_MATRIX_HPP

#include <vector>
#include <iosfwd>

class matrix {
public:
  matrix(int r, int c) : rows_{r}, columns_{c}, buffer_(r*c) {}

  [[nodiscard]] int rows() const { return rows_; }
  [[nodiscard]] int columns() const { return columns_; }

  double & operator()(int i, int j) { return buffer_[index(i,j)]; }
  double operator()(int i, int j) const { return buffer_[index(i,j)]; }

  matrix operator*(const matrix & rhs) const;

private:
  [[nodiscard]] int index(int i, int j) const { return i * columns_ + j; }
private:
  int rows_;
  int columns_;
  std::vector<double> buffer_;
};

std::ostream & operator<<(std::ostream & os, const matrix & m);

#endif // MATMULT_MATRIX_HPP
