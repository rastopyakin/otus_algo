#include "dynamic.hpp"
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
  int N[] = {2'500,  5'000,   10'000,  20'000, 40'000,
             80'000, 160'000, 320'000, 640'000};

    // for (int n = 5'000; n <= 100'000'000; n *= 2) {
  // for (int n : N) {
    // std::cout << n << " " << measure_adding<WrappedDeque<int>>(n) << "\n";
    // std::cout << n << " " << measure_random_adding<WrappedDeque<int>>(n)
              // << "\n";
  // }

  return 0;
}
