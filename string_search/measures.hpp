#ifndef MEASURES_HPP
#define MEASURES_HPP

#include "counter.hpp"
#include "test.hpp"
#include "types.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

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

struct MeasureMode {
  constexpr static struct Random {
  } random{};

  // [pref-char + sequence of repeating fragments + suff char]
  struct Degenerate {
    char pref;
    char sequenced[6];
    char suff;;
  };
  constexpr static Degenerate degenerate_1{0, "a", 0};
  constexpr static Degenerate degenerate_2{'a', "ba", 'c'};

  constexpr static struct SmallAlph { } small_alph{}; };

using MeasureCaseT = std::pair<std::string, std::string>; // text and pattern
MeasureCaseT make_measure_case(pos_type pattern_length, const MeasureMode::Random &);
MeasureCaseT make_measure_case(pos_type pattern_length, const MeasureMode::Degenerate &);
// random [ab] string and fixed [ab] pattern
MeasureCaseT make_measure_case(pos_type pattern_length, const MeasureMode::SmallAlph &);

double measure_string_fun(StringFunT f, size_t n, char pref = 0, char seq = 'a',
                          char suff = 'b');

double measure_search_fun(SearchFunAllT f, std::string_view text, std::string_view pattern);

template <class MeasureMode>
double measure_search_fun(SearchFunAllT f, pos_type pattern_length, const MeasureMode &mode) {

  auto target_fn = [&f, &mode](pos_type n) {
    MeasureCaseT s{make_measure_case(n, mode)};
    return measure_search_fun(f, s.first, s.second);
  };

  return avg_fn_result(1.0e-3, target_fn, pattern_length);
}

inline double measure_search_fun(SearchFunAllT f, pos_type pattern_length,
                                 const MeasureMode::Degenerate &mode) {
  MeasureCaseT s{make_measure_case(pattern_length, mode)};

  return measure_search_fun(f, s.first, s.second);
}

// the following is to measure number of char comparisons made by an algorithm

using CountingString = std::vector<CountingChar>;
using CountingSearchfun = std::function<void(const CountingString &, const CountingString &)>;

double count_comparisons(CountingSearchfun f, const CountingString &text,
                         const CountingString &pattern);

template <class MeasureMode>
double count_comparisons(CountingSearchfun f, pos_type pattern_length, const MeasureMode &mode) {

  auto target_fn = [&f, &mode](pos_type n) {
    CountingString text{}, pattern{};
    {
      MeasureCaseT s{make_measure_case(n, mode)};
      std::copy_n(s.first.begin(), s.first.length(), std::back_inserter(text));
      std::copy_n(s.second.begin(), s.second.length(), std::back_inserter(pattern));
    }

    return count_comparisons(f, text, pattern);
  };

  return avg_fn_result(2.0e-4, target_fn, pattern_length);
}

inline double count_comparisons(CountingSearchfun f, pos_type pattern_length,
                                 const MeasureMode::Degenerate &mode) {
  MeasureCaseT s{make_measure_case(pattern_length, mode)};

  CountingString text{}, pattern{};
  std::copy_n(s.first.begin(), s.first.length(), std::back_inserter(text));
  std::copy_n(s.second.begin(), s.second.length(), std::back_inserter(pattern));

  return count_comparisons(f, text, pattern);
}

#endif /* MEASURES_HPP */
