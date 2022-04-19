#include <iostream>

#include "b_tree.hpp"
#include "measures.hpp"

int main(int argc, char *argv[]) {


  for (int N = 100; N < 2'500'000; N *= 2) {
    // std::cout << N << " " << measure_insert_ordered<BSTree>(N) << "\n";
    // std::cout << N << " " << measure_insert_random<BSTree>(N) << "\n";
    // std::cout << N << " " << measure_search_random<BSTree>(N) << "\n";
    std::cout << N << " " << measure_search_ordered<BSTree>(N) << "\n";
  }

  return 0;
}
