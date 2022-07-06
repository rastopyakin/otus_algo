#include "kmp_search.hpp"
#include "test.hpp"
#include "types.hpp"
#include "counter.hpp"
#include "full_search.hpp"

#include <iostream>
#include <numeric>
#include <string_view>
#include <type_traits>
#include <vector>

std::ostream &operator<<(std::ostream &os, const CountingChar &ch) {

  os << ch.get();
  return os;
}

int main(int argc, char *argv[]) {
  // using n_type = std::make_unsigned_t<std::string_view::value_type>;
  // std::cout << static_cast<unsigned>(pattern[pattern.length()]) << "\n";

  auto pf = [](std::string_view s){
    std::vector<size_t> prefix(s.length());
    pi_function(s.begin(), s.end(), prefix);
    return prefix;
  };

  std::cout << u8"π-function comparative test: ";
  // test_comparative(v0::pi_function_naive, v0::pi_function);
  test_comparative(v0::pi_function_naive, pf);

  std::cout << "KMP Test:\n";
  test_complex(kmp_search);

  std::cout << "KMP Test v0 var 1:\n";
  test_complex(v0::kmp_search_1);

  std::cout << "KMP Test v0 var 2:\n";
  test_complex(v0::kmp_search_2);

  // std::cout << "KMP Test var 3:\n";
  // test_complex(v0::kmp_search_3);

  std::cout << "All occurrences version:\n";
  test_complex(kmp_search_all);

  std::cout << "All occurrences v0 var 2 version:\n";
  test_complex(v0::kmp_search_all_2);

  using namespace std::placeholders;

  std::cout << "\nComparative verification:\n";
  std::cout << "\"abc\" pattern: ";
  test_comparative(std::bind(v0::full_search_all, _1, "abc"),
                   std::bind(kmp_search_all, _1, "abc"));

  std::cout << "\"aaa\" pattern: ";
  test_comparative(std::bind(v0::full_search_all, _1, "aaa"),
                   std::bind(kmp_search_all, _1, "aaa"));

  std::cout << "\"aba\" pattern: ";
  test_comparative(std::bind(v0::full_search_all, _1, "aba"),
                   std::bind(kmp_search_all, _1, "aba"));

  return 0;
}
