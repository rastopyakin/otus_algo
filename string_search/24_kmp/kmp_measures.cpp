#include "measures.hpp"
#include "types.hpp"
#include "kmp_search.hpp"

#include <iostream>

int main(int argc, char *argv[]) {

  auto counting_fn = [] (const CountingString &t, const CountingString &p) {
    // kmp_search_1(t.begin(), t.end(), p.begin(), p.end(), [](auto){});
    kmp_search_2(t.begin(), t.end(), p.begin(), p.end(), [](auto){});
  };


  for (pos_type length = 10; length < 2000'000; length *= 2) {
    std::cout << length << " ";

    // baa....a pattern
    // std::cout << measure_fun(suffix_shifts_naive, n, 'b', 'a', 0);
    // std::cout << measure_string_fun(v0::suffix_shifts, n, 'b', 'a', 0);

    // std::cout << measure_string_fun(
    // [](std::string_view s) {
    // std::vector<pos_type> sft(s.length() + 1, s.length());
    // suffix_table(s.begin(), s.end(), sft);
    // return sft;
    // },
    // n);

    // std::cout << measure_search_fun(kmp_search_all, length, MeasureMode::small_alph);
    std::cout << measure_search_fun(kmp_search_all, length, MeasureMode::random);
    // std::cout << measure_search_fun(kmp_search_all, length, MeasureMode::degenerate);

    // std::cout << count_comparisons(counting_fn, length, MeasureMode::random);
    // std::cout << count_comparisons(counting_fn, length, MeasureMode::small_alph);
    // std::cout << count_comparisons(counting_fn, length, MeasureMode::degenerate);

    std::cout << "\n";
  }

  return 0;
}
