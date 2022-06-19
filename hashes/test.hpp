#ifndef TEST_HPP
#define TEST_HPP

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using Key = std::string;
using Value = int;
using Reference_HT = std::unordered_map<Key, Value>; // just use standart implementation for tests

using KV_Pair = std::pair<Key, Value>; // don't need const on Key type for testing purposes

std::vector<KV_Pair> make_test_insertions(size_t n_elem = 50, std::size_t str_len = 5);

inline void report(bool passed) {
  const char ANSI_RED_BEGIN[] = "\x1B[31m";
  const char ANSI_COLOR_END[] = "\033[0m";
  const char ANSI_GREEN_BEGIN[] = "\x1B[32m";

  std::cout << (passed ? ANSI_GREEN_BEGIN : ANSI_RED_BEGIN);
  std::cout << (passed ? "PASSED" : "FAILED") << ANSI_COLOR_END << "\n";
}

// cout any container: usable for testing
template <class T, template <class, class...> class Cont, class... Rest>
auto operator<<(std::ostream &os, const Cont<T, Rest...> &v)
    -> std::enable_if_t<!std::is_same_v<T, char>, std::ostream &> {

  std::copy(std::begin(v), std::end(v), std::ostream_iterator<T>{os, ", "});
  return os;
}

template <template <class, class> class HT> void insert_test(std::size_t n_elem = 50'000) {

  bool passed = true;
  std::vector<KV_Pair> pairs_to_insert{make_test_insertions(n_elem)};
  HT<Key, Value> tested_table;
  Reference_HT ref_table;

  auto insert_and_check = [](auto &t_1, auto &t_2, auto &k, auto &v) -> bool {
    auto [it_1, inserted_1] = t_1.insert_or_assign(k, v);
    auto [it_2, inserted_2] = t_2.insert_or_assign(k, v);

    if (inserted_1 != inserted_2) {
      std::cout << "Wrong insertion flag\n";
      return false;
    }

    if (*it_1 != *it_2) {
      std::cout << "insert_or_assign(...) returns wrong iterator or value\n";
      return false;
    }

    return true;
  };

  for (const auto &[s, n] : pairs_to_insert) {
    passed = insert_and_check(tested_table, ref_table, s, n);
    if (!passed)
      break;
  }

  // change values and repeat insertion

  std::for_each(pairs_to_insert.begin(), pairs_to_insert.end(), [](KV_Pair &p) {
    p.second += 10; // why not 10?
  });

  for (const auto &[s, n] : pairs_to_insert) {
    passed = insert_and_check(tested_table, ref_table, s, n);
    if (!passed) {
      std::cout << "On inserting existing values\n";
      break;
    }
  }

  // check sizes
  if (ref_table.size() != tested_table.size()) {
    passed = false;
    std::cout << "Wrong size after insertions\n";
  }

  report(passed);
}

// check the iterating : each inserted key should be accessible and no more than once
template <template <class, class> class HT> void iterator_test(std::size_t n_elem = 50'000) {

  bool passed = true;
  std::vector<KV_Pair> pairs_to_insert{make_test_insertions(n_elem)};
  HT<Key, Value> tested_table;

  for (const auto &[s, n] : pairs_to_insert) {
    tested_table.insert_or_assign(s, n);
  }

  std::vector<KV_Pair> inserted_pairs;
  inserted_pairs.reserve(n_elem);

  std::copy(tested_table.begin(), tested_table.end(), std::back_inserter(inserted_pairs));

  auto pred_less = [](const auto &p1, const auto &p2) -> bool { return p1.first < p2.first; };
  auto pred_compare = [](const auto &p1, const auto &p2) -> bool { return p1.first == p2.first; };

  // transform the test sequence so that it becomes comparable to what we expect from the hashtable

  // sort to have the same order in both keeping the order of insertion
  std::stable_sort(pairs_to_insert.begin(), pairs_to_insert.end(), pred_less);

  // exclude unique keys leaving the last inserted therefore we iterate in reverse
  auto rev_last_unique =
      std::unique(pairs_to_insert.rbegin(), pairs_to_insert.rend(), pred_compare);

  std::sort(inserted_pairs.begin(), inserted_pairs.end(), pred_less);

  passed = std::equal(inserted_pairs.begin(), inserted_pairs.end(), rev_last_unique.base());

  report(passed);

  if (!passed) {
    std::cout << "Sample size: " << std::distance(rev_last_unique.base(), pairs_to_insert.end())
              << "\n";
    std::cout << "Inserted size: " << inserted_pairs.size() << "\n";
  }
}

template <template <class, class> class HT> void operator_access_test(std::size_t n_elem = 50'000) {

  bool passed = true;
  std::vector<KV_Pair> pairs_to_insert{make_test_insertions(n_elem)};
  HT<Key, Value> tested_table;
  Reference_HT ref_table;

  const std::size_t n_elm_w_value = n_elem/2; // number elems to assign immediately
  auto elm_w_value_end = pairs_to_insert.begin() + n_elm_w_value;

  auto pred_less = [](const auto &p1, const auto &p2) -> bool { return p1.first < p2.first; };
  auto pred_compare = [](const auto &p1, const auto &p2) -> bool { return p1.first == p2.first; };

  for (auto i = pairs_to_insert.begin(); i != elm_w_value_end; i++)
    tested_table[i->first] = i->second;

  // compare as in iterator_test()
  std::vector<KV_Pair> inserted_pairs;
  inserted_pairs.reserve(n_elm_w_value);
  std::copy(tested_table.begin(), tested_table.end(), std::back_inserter(inserted_pairs));
  std::sort(inserted_pairs.begin(), inserted_pairs.end(), pred_less);

  std::stable_sort(pairs_to_insert.begin(), elm_w_value_end, pred_less);

  // exclude unique keys leaving the last inserted therefore we iterate in reverse
  auto rev_last_unique =
    std::unique(std::reverse_iterator{elm_w_value_end}, pairs_to_insert.rend(), pred_compare);

  passed = std::equal(inserted_pairs.begin(), inserted_pairs.end(), rev_last_unique.base());

  if (!passed) {
    std::cout << "assigning/inserting via operator[] fails\n";
    report(passed);
  }

  // check the default value insertion
  const std::tuple_element_t<1, KV_Pair> default_value{};
  for (auto i = elm_w_value_end; i != pairs_to_insert.end(); i++) {
    if (tested_table[i->first] != default_value) {
      // maybe we found a duplicate?
      auto mb_dup = std::find_if(std::reverse_iterator{elm_w_value_end}, pairs_to_insert.rend(),
                                 [&i](const auto &p) { return p.first == i->first; });

      if (tested_table[i->first] != mb_dup->second) {
        report(false);
        std::cout << "operator [] yields non-default value\n";
        return;
      }
    } else
      i->second = default_value; // for the purpose of subsequent comparison
  }

  report(passed);
}
#endif /* TEST_HPP */
