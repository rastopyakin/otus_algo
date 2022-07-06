#include "sort.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

template <class T, template <class, class...> class Cont, class... Rest>
auto operator<<(std::ostream &os, const Cont<T, Rest...> &v)
    -> std::enable_if_t<!std::is_same_v<Cont<T, Rest...>, std::string>,
                        std::ostream &> {

  std::copy(std::begin(v), std::end(v), std::ostream_iterator<T>{os, " "});
  return os;
}

int main(int argc, char *argv[]) {

  std::vector<int> v;

  std::mt19937 gen{};
  std::random_device rd;
  gen.seed(rd());

  int n_elem = 20;
  std::uniform_int_distribution<int> d{0, n_elem};

  std::generate_n(std::back_inserter(v), n_elem,
                  [&d, &gen]() { return d(gen); });

  std::sort(v.begin(), v.end());

  std::uniform_int_distribution<int>::param_type p{v.front(), v.back() * 2};
  d.param(p);
  int value = d(gen);

  std::cout << "value: " << value << "\n";

  auto b1 = upper_bound_linear(value, v.begin(), v.end());
  auto b2 = std::upper_bound(v.begin(), v.end(), value);

  std::cout << v << "\n";

  int n_spaces = 0;
  for (auto it = v.begin(); it != b2; it++) {
    n_spaces += std::to_string(*it).length() + 1;
  }

  std::cout << std::string(n_spaces, ' ') << "^\n";

  std::cout << "b1: " << *b1 << "\n";
  std::cout << "b2: " << *b2 << "\n";

  std::cout << (b1 == b2 ? "Passed" : "Failed") << "\n";

  return 0;
}
