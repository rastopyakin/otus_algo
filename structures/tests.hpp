#ifndef TESTS_HPP
#define TESTS_HPP

#include <vector>
#include <random>
#include "measure_tools.hpp"

template<class Array>
const char* random_add() {
  Array array;
  std::vector<int> vec;

  std::mt19937_64 gen;
  size_t index;

  const int n_elems = 1000;
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
