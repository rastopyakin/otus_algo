#ifndef MEASURE_TOOLS_HPP
#define MEASURE_TOOLS_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <utility>
#include <vector>

// average what function F(...) returns with relative error of 1%
// used here for acquisition of timing data
template <class F, class... Args>
double avg_fn_result(double tolerance, F f, Args&& ... args) {
  double time_avg = 0;
  double time_sqr_avg = 0;

  int n_repeat = 5;
  for (int n = 0; n < n_repeat; n++) {
    double measured = f(std::forward<Args>(args)...);
    time_avg += measured;
    time_sqr_avg += measured * measured;
  }

  time_avg /= n_repeat;
  time_sqr_avg /= n_repeat;

  double variance = time_sqr_avg - time_avg*time_avg;
  double rel_error = std::sqrt(variance/n_repeat)/time_avg;

  while (rel_error > tolerance) {
    double time = 0;
    double time_sqr = 0;
    for (int n = 0; n < n_repeat; n++) {
      double measured = f(std::forward<Args>(args)...);
      time += measured;
      time_sqr += measured*measured;
    }
    time /= n_repeat;
    time_sqr /= n_repeat;

    time_avg = (time_avg + time)/2.0; // both time and time_avg has the same number of samples
    time_sqr_avg = (time_sqr_avg + time_sqr)/2.0;
    n_repeat *= 2;              // we collected another n_repeat measurements
    variance = time_sqr_avg - time_avg*time_avg;
    rel_error = std::sqrt(variance/n_repeat)/time_avg;
  }

  return time_avg;
}

// measure execution time of add_back() operation
template <class Array> double add_back_n(long N) {
  namespace chr = std::chrono;
  chr::time_point<chr::high_resolution_clock> start, finish;
  Array arr;
  start = chr::high_resolution_clock::now();
  for (int i = 0; i < N; i++)
    arr.add_back(i);
  finish = chr::high_resolution_clock::now();
  return (finish - start).count();
}

template <class Array> double measure_adding(long N) {
  return avg_fn_result(1.0e-2, add_back_n<Array>, N);
}

// same as uniform_distribution_int<size_t> but just a function
template <class Gen> size_t scale_gen(Gen &g, size_t a, size_t b) {
  constexpr typename Gen::result_type g_range = Gen::max() - Gen::min();
  size_t range = b - a + 1;
  double scale = static_cast<double>(range) / static_cast<double>(g_range);
  double retval = a + scale * (g() - Gen::min());
  return static_cast<size_t>(retval);
}

template <class Array>
double random_add_n(long N, const std::vector<size_t> &indexes) {
  namespace chr = std::chrono;
  chr::time_point<chr::high_resolution_clock> start, finish;
  Array arr;
  start = chr::high_resolution_clock::now();
  for (int i = 0; i < N; i++) {
    arr.add(i, indexes[i]);
  }
  finish = chr::high_resolution_clock::now();
  return (finish - start).count();
}

// measure adding an element to a random pos (0 <= pos <= array size)
template <class Array> double measure_random_adding(long N, double tolerance = 1.0e-2) {
  std::mt19937_64 gen{};

  // random indexes to add an element to array of growing size
  std::vector<size_t> random_index;
  random_index.reserve(N);
  std::generate_n(
      std::back_inserter(random_index), N,
      [&gen, size = 0]() mutable { return scale_gen(gen, 0, size++); });

  return avg_fn_result(tolerance, random_add_n<Array>,  N, random_index);
}

template <class Array, class Generator>
double access_time(Array &arr, Generator &gen, int n_attempts) {
  double time = 0;
  namespace chr = std::chrono;
  chr::time_point<chr::high_resolution_clock> start, finish;

  std::uniform_int_distribution<size_t> dist {0, arr.size() - 1};
  size_t index = dist(gen);
  // std::cerr << "index : " << index << "\n";
  for (int i = 0; i < n_attempts; i++) {
    start = chr::high_resolution_clock::now();
    // increment needed to prevent the .get() call to be optimized out
    arr.get(index)++;
    finish = chr::high_resolution_clock::now();
    time += (finish - start).count();
  }

  return time / n_attempts;
}

template <class Array>
double measure_mean_random_access(int array_size, double tolerance = 1.0e-2) {

  Array arr;
  for (int i = 0; i < array_size; i++)
    arr.add_back(i);

  std::mt19937_64 gen{};
  return avg_fn_result(tolerance, access_time<Array, std::mt19937_64>, arr, gen, 10);
}

#endif /* MEASURE_TOOLS_HPP */
