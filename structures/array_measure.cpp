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

  for (int n = 1'000; n <= 300'000; n *= 1.5) {
    // for (int n : N) {
    std::cout << n << " " << measure_adding_v2<WrappedDeque<int>>(n) << "\n";
    // std::cout << n << " " << measure_mean_random_access<WrappedDeque<int>>(n)
    // << "\n";
    // std::cout << n << " " << measure_random_adding<WrappedDeque<int>>(n)
              // << "\n";
  }

  return 0;
}
