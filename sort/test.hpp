#ifndef TEST_HPP
#define TEST_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

template <class T, template <class, class...> class Cont, class... Rest>
auto operator<<(std::ostream &os, const Cont<T, Rest...> &v)
    -> std::enable_if_t<!std::is_same_v<Cont<T, Rest...>, std::basic_string<T, Rest...>>,
                        std::ostream &> {

  std::copy(std::begin(v), std::end(v), std::ostream_iterator<T>{os, " "});
  return os;
}

template <class Container, class T = typename Container::value_type>
Container make_random_container(T n_elem) {
  Container v;

  std::mt19937 gen{};
  std::random_device rd;
  gen.seed(rd());

  std::uniform_int_distribution<T> d{0, n_elem};

  std::generate_n(std::back_inserter(v), n_elem,
                  [&d, &gen]() { return d(gen); });

  return v;
}

template <template <class> class Sort> void test(int n_elem = 1e4) {

  auto v = make_random_container<std::vector<int>>(n_elem);

  Sort{std::begin(v), std::end(v)};

  std::cout << (std::is_sorted(std::begin(v), std::end(v)) ? "PASSED"
                                                           : "FAILED");
  std::cout << "\n";
}

template <class Sort> void test(Sort sort, int n_elem = 1e4) {

  auto v = make_random_container<std::vector<int>>(n_elem);

  sort(std::begin(v), std::end(v));

  std::cout << (std::is_sorted(std::begin(v), std::end(v)) ? "PASSED"
                                                           : "FAILED");
  std::cout << "\n";
}

#endif /* TEST_HPP */
