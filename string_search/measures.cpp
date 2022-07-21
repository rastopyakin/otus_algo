#include "measures.hpp"
#include "counter.hpp"
#include "types.hpp"
#include "utf8_tools.hpp"

#include <chrono>
#include <cstddef>
#include <numeric>
#include <string>
#include <string_view>

double measure_search_fun(SearchFunAllT f, std::string_view text, std::string_view pattern) {

  pos_collection_type values{};

  auto target_fn = [&f](auto text, auto pattern) {
    pos_collection_type values;
    auto start = std::chrono::high_resolution_clock::now();
    values = f(text, pattern);
    auto finish = std::chrono::high_resolution_clock::now();
    return static_cast<double>((finish - start).count());
  };
  return avg_fn_result(1e-2, target_fn, text, pattern);
}

double measure_string_fun(StringFunT f, size_t n, char pref, char sequenced, char suff) {

  std::string s{};

  if (pref)
    s += pref;
  s += std::string(n, sequenced);
  if (suff)
    s += suff;

  // random strings actually processed by different algorithms in almost equal times
  // std::string s {random_utf8_string(n, u'a', u'b')};

  auto target_fn = [&f](std::string_view s) {
    pos_collection_type values;
    auto start = std::chrono::high_resolution_clock::now();
    values = f(s);
    auto finish = std::chrono::high_resolution_clock::now();

    return static_cast<double>((finish - start).count());
  };

  return avg_fn_result(1e-3, target_fn, s);
}

MeasureCaseT make_measure_case(pos_type pattern_length, const MeasureMode::Random &) {
  return {utf8::random_text(100 * pattern_length, u'a', u'z'),
          utf8::random_text(pattern_length, u'a', u'z')};
}

MeasureCaseT make_measure_case(pos_type pattern_length, const MeasureMode::Degenerate &) {
  return {std::string(100 * pattern_length, 'a'), std::string(pattern_length, 'a')};
}

MeasureCaseT make_measure_case(pos_type pattern_length, const MeasureMode::SmallAlph &) {
  return {utf8::random_text(100 * pattern_length, u'a', u'b'),
          utf8::random_text(pattern_length, u'a', u'b')};
}

double count_comparisons(CountingSearchfun f, const CountingString &text,
                      const CountingString &pattern) {
  f(text, pattern);
  auto acc_op = [](std::size_t i, const CountingChar &ch) { return i + ch.get(); };

  const std::size_t init = 0;

  return std::accumulate(text.begin(), text.end(), init, acc_op) +
         std::accumulate(pattern.begin(), pattern.end(), init, acc_op);
}
