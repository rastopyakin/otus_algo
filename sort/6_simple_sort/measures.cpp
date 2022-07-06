#include <iostream>

#include "measures.hpp"
#include "sort.hpp"

int main(int argc, char *argv[])
{
  for (int N = 100; N < 4e6; N *= 2) {
    // std::cout << N << " " << measure<bubble_sort_v1>(N) << "\n";
    // std::cout << N << " " << measure<bubble_sort_v2>(N) << "\n";

    // std::cout << N << " " << measure<insertion_sort_v1>(N) << "\n";
    std::cout << N << " " << measure<insertion_sort_v1_5>(N) << "\n";
    // std::cout << N << " " << measure<insertion_sort_v2>(N) << "\n";
    // std::cout << N << " " << measure<insertion_sort_v3>(N) << "\n";

    // std::cout << N << " " << measure<shell_sort_v1>(N) << "\n";
    // std::cout << N << " " << measure<shell_sort_v2>(N) << "\n";
    // std::cout << N << " " << measure<shell_sort_v3>(N) << "\n";
  }
  return 0;
}
