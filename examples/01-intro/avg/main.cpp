#include <chrono>
#include <iostream>
#include <random>

#include "seq.hpp"
#include "stl.hpp"
#include "stl_par.hpp"
#include "tbb.hpp"
#include "tbbstl.hpp"

template<typename R>
std::vector<int> make_vector(int n, R &rng) {
  constexpr int min_value = 0;
  constexpr int max_value = 100;
  std::uniform_int_distribution<int> dist{min_value, max_value};
  std::vector<int> result;
  result.reserve(n);
  std::generate_n(std::back_inserter(result), n, [&] { return dist(rng); });
  return result;
}

struct timing {
  std::chrono::microseconds elapsed_square;
  std::chrono::microseconds elapsed_average;

  auto total() const {
    return elapsed_square + elapsed_average;
  }
};

template <typename FS, typename FA>
timing run_sequential(FS square, FA average) {
  constexpr int max = 100'000'000;

  std::mt19937_64 rng{0};
  auto v = make_vector(max, rng);

  using clk = std::chrono::high_resolution_clock;
  auto t0 = clk::now();
  square(v);

  auto t1 = clk::now();
  auto av = average(v);

  auto t2 = clk::now();

  std::cout << "size = " << v.size() << "\n";
  std::cout << "average = " << av << "\n";

  return timing{
      .elapsed_square = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0),
      .elapsed_average = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
  };
}

void print_time(std::string_view name, timing timing_result) {
  std::cout << name << "\n";
  std::cout << "  square = " << timing_result.elapsed_square.count() << "\n";
  std::cout << "  average = " << timing_result.elapsed_average.count() << "\n";
  std::cout << "  total= " << timing_result.total().count() << "\n";
}

void print_speedup(std::string_view name, timing base, timing value) {
  double square_speed = double(base.elapsed_square.count()) / double(value.elapsed_square.count());
  double average_speed = double(base.elapsed_average.count()) / double(value.elapsed_average.count());
  double speedup =
      double (base.total().count()) / double(value.total().count());
  std::cout << name << "\n";
  std::cout << "  square = " << value.elapsed_square.count()
            << "[speedup=" << square_speed << "]\n";
  std::cout << "  average = " << value.elapsed_average.count()
            << "[speedup=" << average_speed << "]\n";
  std::cout << "  total = " << value.total().count()
            << "[speedup=" << speedup << "]\n";
}


int main() {
  auto tseq = run_sequential(square_seq, average_seq);
  auto tstl = run_sequential(square_stl, average_stl);
  auto tstlpar = run_sequential(square_stlpar, average_stlpar);
  auto tstlunseq = run_sequential(square_stlunseq, average_stlunseq);
  auto tstlparunseq = run_sequential(square_stlparunseq, average_stlparunseq);
  auto ttbb = run_sequential(square_tbb, average_tbb);
  auto ttbbstl = run_sequential(square_tbbstl, average_tbbstl);
  print_time("Sequential", tseq);
  print_speedup("STL Algorithms", tseq, tstl);
  print_speedup("STL Parallel Algorithms", tseq, tstlpar);
  print_speedup("STL Unsequenced Algorithms", tseq, tstlunseq);
  print_speedup("STL Parallel Unsequenced Algorithms", tseq, tstlparunseq);
  print_speedup("TBB basic", tseq, ttbb);
  print_speedup("TBB+STL", tseq, ttbbstl);
}