#include "dynamic_array.hpp"
#include "measure_tools.hpp"
#include "wrapper.hpp"

#include <deque>
#include <forward_list>
#include <iostream>
#include <list>

template <class T> using SingleArray = Array<T, SingleAlloc<T>>;
template <class T> using VectorArray = Array<T, VectorAlloc<T>>;
template <class T> using FactorArray = Array<T, FactorAlloc<T>>;

template <class T> using WrappedVector = Wrapper<std::vector<T>>;
template <class T> using WrappedList = Wrapper<std::list<T>>;
template <class T> using WrappedDeque = Wrapper<std::deque<T>>;

int main(int argc, char *argv[]) {

  // std::cout << "10'000'000 : " << measure_mean_random_access_v2<WrappedVector<int>>(10'000'000) << "\n";
  // std::cout << "100 : " << measure_mean_random_access<WrappedVector<int>>(100) << "\n";
  // std::cout << "4588 : " << measure_mean_random_access_v2<WrappedVector<int>>(4588) << "\n";
  // std::cout << "700'000 : " << measure_mean_random_access_v2<WrappedVector<int>>(700'000) << "\n";

  for (int n = 1'000; n <= 30'000'000; n *= 1.10) {
    // for (int n : N) {
    std::cout << n << " " << measure_adding<SingleArray<int>>(n) << "\n";
    // std::cout << n << " " << measure_mean_random_access<FactorArray<int>>(n, 2.5e-3)
              // << "\n";
    // std::cout << n << " " << measure_random_adding<SingleArray<int>>(n)
    // << "\n";
  }

  return 0;
}
