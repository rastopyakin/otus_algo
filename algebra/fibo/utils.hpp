#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <iostream>
#include <array>

template <class T> constexpr T power(T x, unsigned long n, T x0 = 1) {
  T result = x0;
  T x_2 = x;

  while (n) {
    if (n % 2)
      result *= x_2;
    n /= 2;
    x_2 *= x_2;
  }

  return result;
}

class big_integer {
public:
  using base_type = unsigned long;
  static constexpr base_type base_log10 = 9;
  static constexpr base_type base = power(10ul, base_log10);

public:
  big_integer() = default;
  big_integer(base_type value) : digits{value} {}
  big_integer(const big_integer &other) = default;
  big_integer(big_integer &&other) = default;
  big_integer &operator=(const big_integer &other) = default;
  big_integer &operator=(big_integer &&other) noexcept = default;
  ~big_integer() = default;

  void operator+=(const big_integer &other);

  friend big_integer operator*(const big_integer &lhs, const big_integer &rhs);

  friend big_integer operator+(big_integer lhs, const big_integer &rhs) {
    lhs += rhs;
    return lhs;
  }
  friend std::ostream &operator<<(std::ostream &os, const big_integer &value);

private:
  std::vector<base_type> digits;
};

template <class T> class matrix22 {
public:
  using index_type = unsigned long;
  using elem_type = T;

public:
  template <class... Args> matrix22(Args... args) : elems{args...} {}

  T operator()(index_type row, index_type col) const {
    return elems[2 * row + col];
  }

  static matrix22<T> eye() { return matrix22<T>{T(1), T(0), T(0), T(1)}; }

  void operator*=(const matrix22 &other) {
    auto &self = *this;
    matrix22<T> tmp{self(0, 0) * other(0, 0) + self(0, 1) * other(1, 0),
                    self(0, 0) * other(0, 1) + self(0, 1) * other(1, 1),
                    self(1, 0) * other(0, 0) + self(1, 1) * other(1, 0),
                    self(1, 0) * other(0, 1) + self(1, 1) * other(1, 1)};
    self = std::move(tmp);
  }

private:
  std::array<T, 4> elems;
};

template <class T>
std::ostream &operator<<(std::ostream &os, const matrix22<T> matrix) {

  os << matrix(0, 0) << " " << matrix(0, 1) << "\n";
  os << matrix(1, 0) << " " << matrix(1, 1) << "\n";
  return os;
}


#endif /* UTILS_HPP */
