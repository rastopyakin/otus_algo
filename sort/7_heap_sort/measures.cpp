#include "measures.hpp"
#include "sort.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  for (int n = 100; n < 3e6; n *= 2) {
    // std::cout << n << " " << measure([](auto f, auto l) { selection_sort(f, l); }, n) << "\n";
    // std::cout << n << " " << measure([](auto f, auto l) { heapify(f, l); }, n) << "\n";
    std::cout << n << " " << measure([](auto f, auto l) { heap_sort(f, l); }, n) << "\n";
  }

  return 0;
}
