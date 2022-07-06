#include "full_search.hpp"
#include "test.hpp"
#include <iostream>
#include <functional>

int main(int argc, char *argv[]) {

  std::cout << "Full Search Test (v0) :\n";
  test_complex(v0::full_search);
  std::cout << "All occurences version:\n";
  test_complex(v0::full_search_all);
  std::cout << "\n";

  std::cout << "Full Search Test :\n";
  test_complex(full_search);
  std::cout << "All occurences version:\n";
  test_complex(full_search_all);
  std::cout << "\n";

  using namespace std::placeholders;
  std::cout << "Comparative verification:\n";
  std::cout << "\"abc\" pattern: ";
  test_comparative(std::bind(v0::full_search_all, _1, "abc"),
                   std::bind(full_search_all, _1, "abc"));

  std::cout << "\"aaa\" pattern: ";
  test_comparative(std::bind(v0::full_search_all, _1, "aaa"),
                   std::bind(full_search_all, _1, "aaa"));

  std::cout << "\"aba\" pattern: ";
  test_comparative(std::bind(v0::full_search_all, _1, "aba"),
                   std::bind(full_search_all, _1, "aba"));

  return 0;
}
