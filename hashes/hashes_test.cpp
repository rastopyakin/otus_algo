#include "hash_table.hpp"
#include "test.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// template <> struct std::hash<type> {
// size_t operator()(const type &t) { return reinterpret_cast<size_t>(&t); }
// };

struct no_default_cnstr {
  no_default_cnstr() = delete;
  no_default_cnstr(const no_default_cnstr &) = default;
  no_default_cnstr(int value) : _value(value) {}
  int _value = 0;

  friend std::ostream &operator<<(std::ostream &os, const no_default_cnstr &t) {
    os << t._value;
    return os;
  }
};

using HT = HashTable<std::string, int>;
std::ostream &operator<<(std::ostream &os, const HT::node_type &node) {
  os << node.key() << " <-> " << node.value();
  return os;
}

int main(int argc, char *argv[]) {

  std::cout << "insertion test : ";
  insert_test<HashTable>();

  std::cout << "iterating test : ";
  iterator_test<HashTable>();

  std::cout << "operator[] test : ";
  operator_access_test<HashTable>();

  HT table;
  // // std::unordered_map<std::string, int> table;

  auto insert_and_report = [&table](auto k, auto v) {
    auto [it, inserted] = table.insert_or_assign(k, v);
    std::cout << it->first << ": " << (inserted ? "inserted" : "assigned") << ", value "
              << it->second << "\n";
  };

  std::vector<KV_Pair> pairs_to_insert{make_test_insertions(10)};
  std::sort(pairs_to_insert.begin(), pairs_to_insert.end(),
            [](const auto &p1, const auto &p2) -> bool { return p1.first < p2.first; });

  for (const auto& [s, n] : pairs_to_insert)
    insert_and_report(s, n);

  // // table.rehash(9);

  // std::cout << "\ntable size : " << table.size() << "\n";
  // // std::cout << "buckets :\n";
  // // for (std::size_t n = 0; n < table.bucket_count(); n++)
  //   // std::cout << n << " -> " << table.bucket(n) << "\n";
  // std::cout << "\n";

  // for (auto i = table.begin(); i != table.end(); ++i)
  //   std::cout << i->first << " " << i->second << "\n";

  // std::cout << "\n";
  // HT words;
  // for (; *argv; argv++)
  //   words[*argv]++;

  // std::hash<std::string> str_hash{};

  // for (const auto &[str, count] : words) {
  //   std::cout << str << " " << count;
  //   std::cout << ", hash: " << str_hash(str) << "\n";
  // }

  // // std::string str {"xxx"};
  // // std::cout << std::_Hash_impl::hash(str.data(), str.length()) << "\n";

  return 0;
}
