#include "bm_search.hpp"
#include "measures.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>

int main(int argc, char *argv[]) {

  for (size_t n = 10; n < 2'000'000; n *= 2) {
    std::cout << n << " ";
    // a....ab pattern
    // std::cout << measure_string_fun(v0::Z_function_naive, n);
    // std::cout << measure_string_fun(v0::Z_function, n);

    // std::cout << measure_string_fun(
    //                  [](std::string_view s) {
    //                    std::vector<pos_type> z(s.length());
    //                    Z_function(s.begin(), s.end(), z);
    //                    return z;
    //                  },
    //                  n);

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

    // std::cout << measure_search_fun(boyer_moore_horspoole_search_all, n, MeasureMode::small_alph);
    // std::cout << measure_search_fun(boyer_moore_horspoole_search_all, n, MeasureMode::degenerate);

    // std::cout << measure_search_fun(boyer_moore_search_all, n, MeasureMode::small_alph);
    // std::cout << measure_search_fun(boyer_moore_search_all, n, MeasureMode::degenerate);
    std::cout << measure_search_fun(boyer_moore_search_all, n, MeasureMode::random);

    std::cout << "\n";
  }
  return 0;
}
