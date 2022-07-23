#include "bm_search.hpp"
#include "counter.hpp"
#include "measures.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>

int main(int argc, char *argv[]) {

  auto counting_fn_bm = [](const CountingString &t, const CountingString &p) {
    boyer_moore_search(t.begin(), t.end(), p.begin(), p.end(), [](auto) {});
  };

  auto counting_fn_bmh = [](const CountingString &t, const CountingString &p) {
    boyer_moore_horspoole_search(t.begin(), t.end(), p.begin(), p.end(), [](auto) {});
  };

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

    // std::cout << measure_search_fun(boyer_moore_horspoole_search_all, n, MeasureMode::small_alph);
    // std::cout << measure_search_fun(boyer_moore_horspoole_search_all, n, MeasureMode::degenerate);
    std::cout << measure_search_fun(boyer_moore_horspoole_search_all, n, MeasureMode::random);

    // std::cout << measure_search_fun(boyer_moore_search_all, n, MeasureMode::small_alph);
    // std::cout << measure_search_fun(boyer_moore_search_all, n, MeasureMode::degenerate);
    // std::cout << measure_search_fun(boyer_moore_search_all, n, MeasureMode::random);

    // std::cout << count_comparisons(counting_fn_bm, n, MeasureMode::degenerate);
    // std::cout << count_comparisons(counting_fn_bm, n, MeasureMode::random);
    // std::cout << count_comparisons(counting_fn_bm, n, MeasureMode::small_alph);

    std::cout << "\n";
  }
  return 0;
}
