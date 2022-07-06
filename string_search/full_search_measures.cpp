#include "full_search.hpp"
#include "measures.hpp"
#include "types.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

int main(int argc, char *argv[]) {

  auto counting_fn = [] (const CountingString &t, const CountingString &p) {
    full_search(t.begin(), t.end(), p.begin(), p.end(), [](auto){});
  };

  auto [text, pattern] = make_measure_case(100, MeasureMode::small_alph);

  CountingString t{}, p{};
  std::copy(pattern.begin(), pattern.end(), std::back_inserter(p));
  std::copy(text.begin(), text.end(), std::back_inserter(t));

  std::cout << "comparisons: " << count_comparisons(counting_fn, t, p) << "\n";
  v0::full_search(text, pattern);

  std::cout << count_comparisons(counting_fn, 1000, MeasureMode::small_alph) << "\n";

  // for (pos_type length = 10; length < 2000'000; length *= 2) {
  //   std::cout << length << " ";
  //   // std::cout << measure_search_fun(v0::full_search_all, length, MeasureMode::random) << "\n";
  //   // std::cout << measure_search_fun(full_search_all, length, MeasureMode::degenerate) << "\n";
  //   std::cout << measure_search_fun(v0::full_search_all, length, MeasureMode::small_alph) << "\n";
  // }

  return 0;
}
