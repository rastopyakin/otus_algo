#ifndef MEASURES_HPP
#define MEASURES_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <memory>
#include <random>
#include <utility>
#include <vector>

// average what function F(...) returns with relative error of 1%
// used here for acquisition of timing data
template <class F, class... Args>
double avg_fn_result(double tolerance, F f, Args &&...args) {
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

    time_avg = (time_avg + time) /
               2.0; // both time and time_avg has the same number of samples
    time_sqr_avg = (time_sqr_avg + time_sqr) / 2.0;
    n_repeat *= 2; // we collected another n_repeat measurements
    variance = time_sqr_avg - time_avg * time_avg;
    rel_error = std::sqrt(variance / n_repeat) / time_avg;
  }

  return time_avg;
}

namespace chr = std::chrono;

template <template <class> class Tree>
double insert_n(const std::vector<int> &numbers) {

  Tree<int> tree;
}

template <template <class> class Tree> double measure_insert_ordered(int N) {

  namespace chr = std::chrono;

  auto target_fn = [N]() {
    Tree<int> tree;
    chr::time_point<chr::high_resolution_clock> start, finish;
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
      tree.insert(i);
    }
    finish = chr::high_resolution_clock::now();
    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(10e-3, target_fn);
}

template <template <class> class Tree> double measure_insert_random(int N) {

  std::mt19937 gen;
  std::uniform_int_distribution<int> d{};

  std::vector<int> numbers;
  std::generate_n(std::back_inserter(numbers), N,
                  [&gen, &d]() { return d(gen); });

  auto target_fn = [&numbers]() {
    Tree<int> tree;
    chr::time_point<chr::high_resolution_clock> start, finish;
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < numbers.size(); i++) {
      tree.insert(numbers[i]);
    }
    finish = chr::high_resolution_clock::now();
    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(10e-3, target_fn);
}

template <template <class> class Tree> double measure_search_random(int N) {

  std::mt19937 gen;
  std::uniform_int_distribution<int> d{};

  std::vector<int> numbers;
  std::generate_n(std::back_inserter(numbers), N,
                  [&gen, &d]() { return d(gen); });

  Tree<int> tree;
  for (int i : numbers)
    tree.insert(i);

  auto target_fn = [&tree, &numbers, &gen](int n) {
    chr::time_point<chr::high_resolution_clock> start, finish;
    std::uniform_int_distribution<size_t> d{0, numbers.size() - 1};
    start = chr::high_resolution_clock::now();
    for (size_t i = 0; i < n; i++)
      tree.search(numbers[d(gen)]);
    finish = chr::high_resolution_clock::now();

    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(10e-3, target_fn, N / 10);
}
template <template <class> class Tree> double measure_search_ordered(int N) {


  Tree<int> tree;
  for (int i = 0; i < N; i++)
    tree.insert(i);

  auto target_fn = [&tree, N]() {
    chr::time_point<chr::high_resolution_clock> start, finish;
    std::mt19937 gen;
    std::uniform_int_distribution<> d{0, N};

    start = chr::high_resolution_clock::now();
    for (int i = 0; i < N/10; i++)
      tree.search(d(gen));
    finish = chr::high_resolution_clock::now();

    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(10e-3, target_fn);
}

#endif /* MEASURES_HPP */
