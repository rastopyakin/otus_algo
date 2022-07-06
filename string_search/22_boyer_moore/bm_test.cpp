#include "bm_search.hpp"
#include "full_search.hpp"
#include "test.hpp"
#include "types.hpp"

#include <functional>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

int main(int argc, char *argv[]) {

  std::cout << "Boyer-Moore-Horspool Test:\n";
  test_complex(boyer_moore_horspoole_search);
  std::cout << "All occurences version:\n";
  test_complex(boyer_moore_horspoole_search_all);
  std::cout << "\n";

  std::cout << "Z-function comparative test: ";

  auto zf = [] (std::string_view s) {
    std::vector<pos_type> z(s.length());
    Z_function(s.begin(), s.end(), z);
    return z;
  };
  test_comparative(v0::Z_function_naive, zf);


  std::cout << "Boyer-Moore suffix shift table comparative test: ";
  auto sf = [] (std::string_view s) {
    std::vector<pos_type> sft(s.length() + 1, s.length());
    suffix_table(s.begin(), s.end(), sft);
    return sft;
  };

  test_comparative(sf, v0::suffix_shifts_naive);

  std::cout << "\nBoyer-Moore Test:\n";
  test_complex(boyer_moore_search);
  std::cout << "All occurences version:\n";
  test_complex(boyer_moore_search_all);

  using namespace std::placeholders;

  test_comparative(std::bind(v0::full_search_all, _1, "abc"),
                   std::bind(boyer_moore_horspoole_search_all, _1, "abc"));

  test_comparative(std::bind(v0::full_search_all, _1, "aaa"),
                   std::bind(boyer_moore_horspoole_search_all, _1, "aaa"));

  test_comparative(std::bind(v0::full_search_all, _1, "aba"),
                   std::bind(boyer_moore_horspoole_search_all, _1, "aba"));

  test_comparative(std::bind(v0::full_search_all, _1, "abc"),
                   std::bind(boyer_moore_search_all, _1, "abc"));

  test_comparative(std::bind(v0::full_search_all, _1, "aaa"),
                   std::bind(boyer_moore_search_all, _1, "aaa"));

  test_comparative(std::bind(v0::full_search_all, _1, "aba"),
                   std::bind(boyer_moore_search_all, _1, "aba"));

  return 0;
}
