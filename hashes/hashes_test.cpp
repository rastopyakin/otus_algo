#include "hash_table.hpp"

#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <tuple>
#include <unordered_map>
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

template <class T, template <class, class...> class Cont, class... Rest>
auto operator<<(std::ostream &os, const Cont<T, Rest...> &v)
    -> std::enable_if_t<!std::is_same_v<T, char>, std::ostream &> {

  std::copy(std::begin(v), std::end(v), std::ostream_iterator<T>{os, ", "});
  return os;
}

using HT = HashTable<std::string, int>;
std::ostream &operator<<(std::ostream &os, const HT::node_type &node) {
  os << node.key() << " <-> " << node.value();
  return os;
}

int main(int argc, char *argv[]) {

  HT table;
  // std::unordered_map<std::string, int> table;

  auto insert_and_report = [&table](auto k, auto v) {
    auto [it, inserted] = table.insert_or_assign(k, v);
    std::cout << it->first << ": " << (inserted ? "inserted" : "assigned") << ", value "
              << it->second << "\n";
  };

  insert_and_report("xxx", 3);
  insert_and_report("yyy", 10);
  insert_and_report("xxx", 30);
  insert_and_report("sasf", 10);
  insert_and_report("sdvfs", 20);
  insert_and_report("dfgvs", 30);
  insert_and_report("gfds", 40);
  insert_and_report("sdfsd", 50);
  insert_and_report("one", 1);
  insert_and_report("two", 2);
  insert_and_report("three", 3);
  insert_and_report("four", 4);
  insert_and_report("five", 5);
  insert_and_report("six", 6);

  // table.rehash(9);

  std::cout << "\ntable size : " << table.size() << "\n";
  // std::cout << "buckets :\n";
  // for (std::size_t n = 0; n < table.bucket_count(); n++)
    // std::cout << n << " -> " << table.bucket(n) << "\n";
  std::cout << "\n";

  for (auto i = table.begin(); i != table.end(); ++i)
    std::cout << i->first << " " << i->second << "\n";

  std::cout << "\n";
  HT words;
  for (; *argv; argv++)
    words[*argv]++;

  std::hash<std::string> str_hash{};

  for (const auto &[str, count] : words) {
    std::cout << str << " " << count;
    std::cout << ", hash: " << str_hash(str) << "\n";
  }

  // std::string str {"xxx"};
  // std::cout << std::_Hash_impl::hash(str.data(), str.length()) << "\n";

  return 0;
}
