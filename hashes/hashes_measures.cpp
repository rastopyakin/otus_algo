#include "hash_table.hpp"
#include "measures.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
  for (int n = 100; n < 1'000'000; n *= 1.1) {
    std::cout << n << " ";

    // std::cout << measure_insert<std::unordered_map>(n);
    std::cout << measure_insert<HashTable_PrimeRehash>(n);
    // std::cout << measure_insert<HashTable_SimpleRehash>(n);

    // std::cout << measure_find<HashTable>(n);
    // std::cout << measure_find<std::unordered_map>(n);

    // std::cout << measure_iterating<HashTable_SimpleRehash>(n, 1000.0f);
    // std::cout << measure_iterating<std::unordered_map>(n);

    std::cout << "\n";
  }

  return 0;
}
