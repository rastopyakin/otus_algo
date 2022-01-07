#include "measure_tools.hpp"

double measure_timing(double (*f)(long), long arg) {
  double time_avg = 0;
  double time_sqr_avg = 0;

  int n_repeat = 5;
  for (int n = 0; n < n_repeat; n++) {
    double measured = f(arg);
    time_avg += measured;
    time_sqr_avg += measured * measured;
  }

  time_avg /= n_repeat;
  time_sqr_avg /= n_repeat;

  double variance = time_sqr_avg - time_avg * time_avg;
  double rel_error = std::sqrt(variance / n_repeat) / time_avg;
  const double tolerance = 0.01;

  while (rel_error > tolerance) {
    double time = 0;
    double time_sqr = 0;
    for (int n = 0; n < n_repeat; n++) {
      double measured = f(arg);
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
