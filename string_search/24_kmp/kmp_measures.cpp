#include "measures.hpp"
#include "types.hpp"
#include "kmp_search.hpp"

#include <iostream>

int main(int argc, char *argv[]) {

  for (pos_type length = 10; length < 2000'000; length *= 2) {
    std::cout << length << " ";
    std::cout << measure_search_fun(v0::kmp_search_all_2, length, MeasureMode::random);
    // std::cout << measure_search_fun(v0::kmp_search_all_3, length, MeasureMode::degenerate);

    // last test running om hfsr
    // std::cout << measure_search_fun(v0::kmp_search_all_2, length, MeasureMode::small_alph);
    std::cout << "\n";
    std::cout << measure_search_fun(kmp_search_all, length, MeasureMode::random);
    std::cout << "\n";
  }

  return 0;
}
