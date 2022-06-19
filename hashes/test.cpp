#include "test.hpp"

#include <algorithm>
#include <iterator>
#include <random>
#include <utility>

std::string make_random_string(std::mt19937 &g, std::size_t n_elem) {

  std::uniform_int_distribution<char> d{'a', 'z'};
  std::string result;

  std::generate_n(std::back_inserter(result), n_elem, [&]() { return d(g); });

  return result;
}
std::vector<KV_Pair> make_test_insertions(std::size_t n_elem, std::size_t str_len) {
  std::mt19937 gen;
  std::random_device rd;
  gen.seed(rd());

  std::vector<KV_Pair> result;
  result.reserve(n_elem);

  std::generate_n(std::back_inserter(result), n_elem, [&, n = 0]() mutable {
    return std::make_pair(make_random_string(gen, str_len), n++);
  });

  return result;
}
