#ifndef MEASURE_TOOLS_HPP
#define MEASURE_TOOLS_HPP

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <random>
#include <vector>
#include <functional>

template <class Array> long measure_adding(int N) {
  namespace chr = std::chrono;
  long time = 0;
  chr::time_point<chr::high_resolution_clock> start, finish;
  const int n_repeat = 5;
  for (int n = 0; n < n_repeat; n++) {
    Array arr;
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
      arr.add_back(i);
    finish = chr::high_resolution_clock::now();
    time += chr::duration_cast<chr::microseconds>(finish - start).count();
  }
  return time / n_repeat;
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
  std::generate_n(std::back_inserter(random_index), N,
                  [&gen, i = 0]() mutable { return scale_gen(gen, 0, i++); });

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

template <class Array> long measure_random_access(const Array &arr, int N) {
  ulong time = 0;
  namespace chr = std::chrono;
  chr::time_point<chr::high_resolution_clock> start, finish;

  // random indexes to add an element at for each array size
  std::vector<size_t> random_index;
  random_index.reserve(N);
  std::mt19937_64 gen{};
  std::uniform_int_distribution<> dist{0, N - 1};
  std::generate_n(std::back_inserter(random_index), N, std::bind(dist, gen));

  typename Array::value_type got;

  const int n_repeat = 5;
  for (int i = 0; i < n_repeat; i++) {
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
      got = arr.get(random_index[i]);
    }
    finish = chr::high_resolution_clock::now();
    time += chr::duration_cast<chr::microseconds>(finish - start).count();
  }
  return time / n_repeat;
}

#endif /* MEASURE_TOOLS_HPP */
