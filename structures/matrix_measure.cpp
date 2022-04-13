#include <iostream>

#include "dynamic_array.hpp"
#include "matrix_array.hpp"
#include "measure_tools.hpp"

template <class T> using FactorArray = Array<T, FactorAlloc<T>>;
template <class T> using FactorMatrix = MatrixArray<T, FactorArray>;

int main(int argc, char *argv[]) {
  for (int n = 1'000; n <= 30'000'000; n *= 1.1) {
    // for (int n : N) {
    // std::cout << n << " " << measure_adding<FactorMatrix<int>>(n) << "\n";
    std::cout << n << " " << measure_random_adding<FactorMatrix<int>>(n)
              << "\n";
    // std::cout << n << " " << measure_mean_random_access<FactorMatrix<int>>(n, 2.5e-3)
              // << "\n";
  }
  return 0;
}
