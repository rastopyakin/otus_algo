#ifndef MEASURES_HPP
#define MEASURES_HPP

#include <chrono>
#include <cmath>
#include <vector>

#include "test.hpp"

template <class F, class... Args>
double avg_fn_result(double tolerance, F f, Args &&...args) {
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

    time_avg = (time_avg + time) /
               2.0; // both time and time_avg has the same number of samples
    time_sqr_avg = (time_sqr_avg + time_sqr) / 2.0;
    n_repeat *= 2; // we collected another n_repeat measurements
    variance = time_sqr_avg - time_avg * time_avg;
    rel_error = std::sqrt(variance / n_repeat) / time_avg;
  }

  return time_avg;
}

template <template <class> class Sort> double measure(int n_elem) {

  auto v = make_random_container<std::vector<int>>(n_elem);

  auto target_fn = [n_elem, &v](){
    std::vector v_copy {v};
    std::chrono::time_point<std::chrono::high_resolution_clock> start, finish;

    start = std::chrono::high_resolution_clock::now();
    Sort{std::begin(v_copy), std::end(v_copy)};
    finish = std::chrono::high_resolution_clock::now();

    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(0.5e-2, target_fn);
}

template <class Sort> double measure(Sort &&sort, int n_elem) {

  auto v = make_random_container<std::vector<int>>(n_elem);

  auto target_fn = [n_elem, &v, &sort](){
    auto v_copy {v};
    std::chrono::time_point<std::chrono::high_resolution_clock> start, finish;

    start = std::chrono::high_resolution_clock::now();
    sort(std::begin(v_copy), std::end(v_copy));
    finish = std::chrono::high_resolution_clock::now();

    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(0.5e-2, target_fn);
}

#endif /* MEASURES_HPP */
