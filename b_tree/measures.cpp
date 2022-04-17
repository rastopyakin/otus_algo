#include <iostream>

#include "b_tree.hpp"
#include "measures.hpp"

int main(int argc, char *argv[]) {


  for (int N = 1'000; N < 200'000; N *= 2) {
    // std::cout << N << " " << measure_insert_ordered<BSTree>(N) << "\n";
    std::cout << N << " " << measure_insert_random<BSTree>(N) << "\n";
  }
  return 0;
}
