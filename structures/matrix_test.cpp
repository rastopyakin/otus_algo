#include <iostream>

#include "dynamic_array.hpp"
#include "matrix_array.hpp"
#include "tests.hpp"

template <class T> using FactorArray = Array<T, FactorAlloc<T>>;
template <class T> using FactorMatrix = MatrixArray<T, FactorArray>;

int main(int argc, char *argv[]) {

  std::cout << "Factor matrix random add : " << random_add<FactorMatrix<int>>(100'000);

  return 0;
}
