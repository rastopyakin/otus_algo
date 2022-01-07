#ifndef TESTS_HPP
#define TESTS_HPP

#include <deque>
#include <random>
#include "measure_tools.hpp"

template<class Array>
const char* random_add(int n_elems = 1000) {
  Array array;
  std::deque<int> vec;

  std::mt19937_64 gen;
  size_t index;

  for (int i = 0; i < n_elems; i++) {
    index = scale_gen(gen, 0, i);
    array.add(i, index);
    vec.insert(vec.cbegin() + index, i);
  }

  bool ok = true;
  for (int i = 0; i < n_elems; i++)
    if (vec[i] != array.get(i)) {
      ok = false;
      break;
    }
  const char *out = ok ? "OK\n" : "FAIL\n";
  return out;
}


#endif /* TESTS_HPP */
