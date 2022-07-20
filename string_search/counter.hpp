#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <cstddef>
#include <functional>
class CountingChar {
public:
  CountingChar() = default;
  CountingChar(char _ch) : ch{_ch} {}
  CountingChar(const CountingChar &other) = default;

  unsigned long get() const { return counter; }
  void reset() { counter = 0; }
  operator char() { return ch; }
  operator char() const { return ch; }

  friend bool operator==(const CountingChar &ch1, const CountingChar &ch2) {
    ch1.counter++;
    return ch1.ch == ch2.ch;
  }

  friend bool operator!=(const CountingChar &ch1, const CountingChar &ch2) {
    ch1.counter++;
    return ch1.ch != ch2.ch;
  }

  auto hash() const { return std::hash<char>{}(ch); }

private:
  mutable unsigned long counter = 0;
  char ch = 0;
};

namespace std {
template <> struct hash<CountingChar> {
  std::size_t operator()(const CountingChar &ch) const { return ch.hash(); }
};
} // namespace std
#endif /* COUNTER_HPP */
