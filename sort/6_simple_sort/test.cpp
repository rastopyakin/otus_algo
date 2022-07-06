#include "test.hpp"
#include "sort.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  std::cout << "Bubble v1: ";
  test<bubble_sort_v1>();
  std::cout << "Bubble v2: ";
  test<bubble_sort_v2>();
  std::cout << "Insertion v1: ";
  test<insertion_sort_v1>();
  std::cout << "Insertion v1_5: ";
  test<insertion_sort_v1_5>();
  std::cout << "Insertion v2: ";
  test<insertion_sort_v2>();
  std::cout << "Insertion v3: ";
  test<insertion_sort_v3>();
  std::cout << "Shell v1: ";
  test<shell_sort_v1>(1e6);
  std::cout << "Shell v2: ";
  test<shell_sort_v2>(1e6);
  std::cout << "Shell v3: ";
  test<shell_sort_v3>(1e6);

  return 0;
}
