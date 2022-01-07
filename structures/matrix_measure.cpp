#include <iostream>

#include "dynamic_array.hpp"
#include "matrix_array.hpp"
#include "measure_tools.hpp"

template <class T> using FactorArray = Array<T, FactorAlloc<T>>;
template <class T> using FactorMatrix = MatrixArray<T, FactorArray>;

int main(int argc, char *argv[])
{
  int N[] = {2'500,  5'000,   10'000,  20'000, 40'000,
             80'000, 160'000, 320'000, 640'000};

  for (int n = 300'000; n <= 300'000'000; n *= 1.5) {
  // for (int n : N) {
    std::cout << n << " " << measure_adding<FactorMatrix<int>>(n) << "\n";
    // std::cout << n << " " << measure_random_adding<FactorMatrix<int>>(n)
              // << "\n";
    // std::cout << n << " " << measure_mean_random_access<FactorMatrix<int>>(n) << "\n";
  }
  return 0;
}
