#ifndef MEASURES_HPP
#define MEASURES_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include "test.hpp"

// average what function F(...) returns with relative error of 1%
// used here for acquisition of timing data
template <class F, class... Args> double avg_fn_result(double tolerance, F f, Args &&...args) {
  double time_avg = 0;
  double time_sqr_avg = 0;

  int n_repeat = 10;
  for (int n = 0; n < n_repeat; n++) {
    double measured = f(std::forward<Args>(args)...);
    time_avg += measured;
    time_sqr_avg += measured * measured;
  }

  time_avg /= n_repeat;
  time_sqr_avg /= n_repeat;

  double variance = time_sqr_avg - time_avg * time_avg;
  double rel_error = std::sqrt(variance / n_repeat) / time_avg;

  while (rel_error > tolerance) {
    double time = 0;
    double time_sqr = 0;
    for (int n = 0; n < n_repeat; n++) {
      double measured = f(std::forward<Args>(args)...);
      time += measured;
      time_sqr += measured * measured;
    }
    time /= n_repeat;
    time_sqr /= n_repeat;

    time_avg = (time_avg + time) / 2.0; // both time and time_avg has the same number of samples
    time_sqr_avg = (time_sqr_avg + time_sqr) / 2.0;
    n_repeat *= 2; // we collected another n_repeat measurements
    variance = time_sqr_avg - time_avg * time_avg;
    rel_error = std::sqrt(variance / n_repeat) / time_avg;
  }

  return time_avg;
}

namespace chr = std::chrono;

template <template <class, class> class HT, class... Args>
double measure_insert(int N, Args... args) {

  auto target_fn = [N, args...]() {
    std::vector<KV_Pair> pairs_to_insert{make_test_insertions(N)};
    HT<Key, Value> table{args...};
    chr::time_point<chr::high_resolution_clock> start, finish;
    start = chr::high_resolution_clock::now();
    for (const auto &[s, n] : pairs_to_insert) {
      table.insert_or_assign(s, n);
    }
    finish = chr::high_resolution_clock::now();
    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(5e-3, target_fn);
}

template <template <class, class> class HT, class... Args>
double measure_find(int N, Args... args) {

  std::vector<KV_Pair> pairs_to_insert{make_test_insertions(N)};
  HT<Key, Value> table{args...};

  for (const auto &[k, v] : pairs_to_insert)
    table.insert_or_assign(k, v);

  auto target_fn = [&table, &pairs_to_insert](int n) {
    chr::time_point<chr::high_resolution_clock> start, finish;
    typename HT<Key, Value>::iterator it{table.begin()};

    start = chr::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
      it = table.find(pairs_to_insert[i].first); // assigning and somehow using the result prevents
                                                 // the call from being optimized out
    finish = chr::high_resolution_clock::now();

    it->second = Value{};

    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(5e-4, target_fn, N);
}

template <template <class, class> class HT, class... Args>
double measure_iterating(int N, Args... args) {

  std::vector<KV_Pair> pairs_to_insert{make_test_insertions(N)};
  HT<Key, Value> table{args...};

  for (const auto &[k, v] : pairs_to_insert)
    table.insert_or_assign(k, v);

  using iterator = typename HT<Key, Value>::iterator;

  iterator end = table.end();
  Value def_val{};

  auto target_fn = [&]() {
    chr::time_point<chr::high_resolution_clock> start, finish;

    iterator i = table.begin();

    start = chr::high_resolution_clock::now();
    for (; i != end; i++) {
      i->second = def_val; // this prevents the loop from being optimized out
    }
    finish = chr::high_resolution_clock::now();

    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(1.5e-3, target_fn);
}

#endif /* MEASURES_HPP */
