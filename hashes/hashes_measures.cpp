#include "hash_table.hpp"
#include "measures.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
  for (int n = 100; n < 1'000'000; n *= 2) {
    std::cout << n << " ";

    // std::cout << measure_insert<HashTable>(n);
    std::cout << measure_find<HashTable>(n);

    std::cout << "\n";
  }

  return 0;
}
