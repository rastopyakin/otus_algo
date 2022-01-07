#ifndef MEASURE_TOOLS_HPP
#define MEASURE_TOOLS_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>

double measure_timing(double (*f)(long), long arg);

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

template <class Array> double measure_adding_v2(long N) {
  return measure_timing(add_back_n<Array>, N);
}

template <class Array> double measure_adding(long N) {
  double time_avg = 0;
  double time_sqr_avg = 0;

  int n_repeat = 5;
  for (int n = 0; n < n_repeat; n++) {
    double measured = add_back_n<Array>(N);
    time_avg += measured;
    time_sqr_avg += measured*measured;
  }

  time_avg /= n_repeat;
  time_sqr_avg /= n_repeat;

  double variance = time_sqr_avg - time_avg*time_avg;
  double rel_error = std::sqrt(variance/n_repeat)/time_avg;
  const double tolerance = 0.01;

  while (rel_error > tolerance) {
    double time = 0;
    double time_sqr = 0;
    for (int n = 0; n < n_repeat; n++) {
      double measured = add_back_n<Array>(N);
      time += measured;
      time_sqr+= measured*measured;
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

// same as uniform_distribution_int<size_t> but just a function
template <class Gen> size_t scale_gen(Gen &g, size_t a, size_t b) {
  constexpr typename Gen::result_type g_range = Gen::max() - Gen::min();
  size_t range = b - a + 1;
  double scale = static_cast<double>(range) / static_cast<double>(g_range);
  double retval = a + scale * (g() - Gen::min());
  return static_cast<size_t>(retval);
}

// tests adding an element to a random pos (0 <= pos <= array size)
template <class Array> long measure_random_adding(int N) {
  std::mt19937_64 gen{};
  ulong time = 0;
  namespace chr = std::chrono;
  chr::time_point<chr::high_resolution_clock> start, finish;

  // random indexes to add an element at for each array size
  std::vector<size_t> random_index;
  random_index.reserve(N);
  std::generate_n(
      std::back_inserter(random_index), N,
      [&gen, size = 0]() mutable { return scale_gen(gen, 0, size++); });

  const int n_repeat = 5;
  for (int i = 0; i < n_repeat; i++) {
    Array arr;
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
      arr.add(i, random_index[i]);
    }
    finish = chr::high_resolution_clock::now();
    time += chr::duration_cast<chr::microseconds>(finish - start).count();
  }
  return time / n_repeat;
}

template <class Array>
long measure_mean_random_access(int array_size, int n_attempts = 10'000) {
  ulong time = 0;
  namespace chr = std::chrono;
  chr::time_point<chr::high_resolution_clock> start, finish;

  // random indexes for getting access
  std::vector<size_t> random_index;
  random_index.reserve(n_attempts);
  std::mt19937_64 gen{};
  std::uniform_int_distribution<> dist{0, array_size - 1};
  std::generate_n(std::back_inserter(random_index), n_attempts,
                  std::bind(dist, gen));

  // preparing the tested array
  Array arr;
  for (int i = 0; i < array_size; i++)
    arr.add_back(i);

  const int n_repeat = 50;

  for (int i = 0; i < n_repeat; i++) {
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < n_attempts; i++) {
      arr.get(random_index[i])++;
    }
    finish = chr::high_resolution_clock::now();
    time += chr::duration_cast<chr::nanoseconds>(finish - start).count();
  }
  return time / n_repeat;
}

#endif /* MEASURE_TOOLS_HPP */
