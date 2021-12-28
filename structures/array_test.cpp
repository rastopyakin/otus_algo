#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "dynamic.hpp"
#include "measure_tools.hpp"
#include "tests.hpp"

template <class T> using SingleArray = Array<T, SingleAlloc<T>>;
template <class T> using VectorArray = Array<T, VectorAlloc<T>>;
template <class T> using FactorArray = Array<T, FactorAlloc<T>>;

int main(int argc, char *argv[]) {

  std::cout << "single array random add : " << random_add<SingleArray<int>>();
  std::cout << "vector array random add : " << random_add<VectorArray<int>>();
  std::cout << "factor array random add : " << random_add<FactorArray<int>>();
  return 0;
}
