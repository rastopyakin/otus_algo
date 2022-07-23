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

  for (pos_type length = 10; length < 2000'000; length *= 2) {
    std::cout << length << " ";

    std::cout << measure_search_fun(full_search_all, length, MeasureMode::random);
    // std::cout << measure_search_fun(full_search_all, length, MeasureMode::small_alph);
    // std::cout << measure_search_fun(full_search_all, length, MeasureMode::degenerate);

    // std::cout << count_comparisons(counting_fn, length, MeasureMode::random);
    // std::cout << count_comparisons(counting_fn, length, MeasureMode::degenerate);
    // std::cout << count_comparisons(counting_fn, length, MeasureMode::small_alph);

    std::cout << "\n";
  }

  return 0;
}
