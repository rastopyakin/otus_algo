#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "sort.hpp"
#include "test.hpp"

void heap_test() {
  auto v = make_random_container<std::vector<int>>(10'000);
  heapify(v.begin(), v.end());
  std::cout << (std::is_heap(v.begin(), v.begin()) ? "PASSED" : "FAILED") << "\n";
}

int main(int argc, char *argv[]) {

  std::cout << "Selection sort: ";
  test([](auto f, auto l) { selection_sort(f, l); });

  std::cout << "Heapifying: ";
  heap_test();

  std::cout << "Selection sort: ";
  test([](auto f, auto l) { heap_sort(f, l); });

  return 0;
}
