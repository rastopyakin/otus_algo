#include "dynamic.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <vector>

template <class Array> long measure_adding(int N) {
  namespace chr = std::chrono;
  long time = 0;
  chr::time_point<chr::high_resolution_clock> start, finish;
  const int n_repeat = 5;
  for (int n = 0; n < n_repeat; n++) {
    Array arr;
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
      arr.add_back(i);
    finish = chr::high_resolution_clock::now();
    time += chr::duration_cast<chr::microseconds>(finish - start).count();
  }
  return time / n_repeat;
}

// same as uniform_distribution_int<size_t> but just a function
template <class Gen> size_t scale_gen(Gen &g, size_t a, size_t b) {
  constexpr typename Gen::result_type g_range = Gen::max() - Gen::min();
  size_t range = b - a + 1;
  double scale = static_cast<double>(range) / static_cast<double>(g_range);
  double retval = a + scale * (g() - Gen::min());
  return static_cast<size_t>(retval);
}

// tests adding an element to a random pos (0 <= pos <= array size)
template <class Array> long measure_random_adding(int N) {
  std::mt19937_64 gen{};
  ulong time = 0;
  namespace chr = std::chrono;
  chr::time_point<chr::high_resolution_clock> start, finish;

  // random indexes to add an element at for each array size
  std::vector<size_t> random_index;
  random_index.reserve(N);
  std::generate_n(std::back_inserter(random_index), N,
                  [&gen, i = 0]() mutable { return scale_gen(gen, 0, i++); });

  const int n_repeat = 5;
  for (int i = 0; i < n_repeat; i++) {
    Array arr;
    start = chr::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
      arr.add(i, random_index[i]);
    }
    finish = chr::high_resolution_clock::now();
    time += chr::duration_cast<chr::microseconds>(finish - start).count();
  }
  return time / n_repeat;
}

template <class T> using SingleArray = Array<T, SingleAlloc<T>>;
template <class T> using VectorArray = Array<T, VectorAlloc<T>>;
template <class T> using FactorArray = Array<T, FactorAlloc<T>>;

void t();

int main(int argc, char *argv[]) {
  int N[] = {5'000, 10'000, 20'000, 40'000, 80'000, 160'000, 320'000, 640'000};


  // for (int n = 5'000; n <= 100'000'000; n *= 2) {
  for (int n : N) {
    // std::cout << n << " " << measure_adding<FactorArray<int>>(n) << "\n";
    std::cout << n << " " << measure_random_adding<VectorArray<int>>(n) << "\n";
  }
  // t();
  return 0;
}

void t() {
  SingleArray<int> array{};
  std::mt19937_64 gen;
  size_t index;
  for (int i = 0; i < 20; i++) {
    index = scale_gen(gen, 0, i);
    array.add(i, index);
  }

  for (size_t i = 0; i < array.size(); i++)
    std::cout << array.get(i) << ' ';
  std::cout << "\n";
  std::cout << "size : " << array.size() << "\n";

  std::vector<int> vec;
  gen.seed(gen.default_seed);
  for (int i = 0; i < 20; i++) {
    index = scale_gen(gen, 0, i);
    vec.insert(vec.cbegin() + index, i);
  }

  for (size_t i = 0; i < vec.size(); i++)
    std::cout << vec[i] << ' ';
  std::cout << "\n";
  std::cout << "size : " << vec.size() << "\n";

  array.add(42, 5);
  array.add(17, 0);
  array.add(300, array.size());

  for (size_t i = 0; i < array.size(); i++)
    std::cout << array.get(i) << ' ';
  std::cout << "\n";
  std::cout << "size : " << array.size() << "\n";
}
