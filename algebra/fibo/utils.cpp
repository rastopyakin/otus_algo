#include "utils.hpp"

void big_integer::operator+=(const big_integer &other) {
  unsigned int pos = 0;
  bool overflow_flag = false;
  for (auto digit : other.digits) {
    if (pos >= digits.size())
      digits.push_back(0);
    digits[pos] += digit;
    if (digits[pos] >= base) {
      digits[pos] %= base;
      overflow_flag = true;
    }

    pos++;

    if (overflow_flag) {
      if (pos >= digits.size())
        digits.push_back(0);
      digits[pos]++;
      overflow_flag = false;
    }
  }
}

big_integer operator*(const big_integer &lhs, const big_integer &rhs) {
  big_integer::base_type carry_value = 0;
  big_integer retval;
  retval.digits.reserve(lhs.digits.size() + rhs.digits.size());
  big_integer tmp;
  tmp.digits.reserve(retval.digits.capacity());
  big_integer::base_type lhs_pos = 0;

  for (auto lhs_digit : lhs.digits) {
    tmp.digits.assign(lhs_pos++, 0); //
    carry_value = 0;
    for (auto rhs_digit : rhs.digits) {
      big_integer::base_type prod = lhs_digit * rhs_digit;
      big_integer::base_type candidate = prod % big_integer::base + carry_value;
      tmp.digits.push_back(candidate % big_integer::base);
      carry_value = prod / big_integer::base + candidate / big_integer::base;
    }
    if (carry_value)
      tmp.digits.push_back(carry_value);
    retval += tmp;
  }

  return retval;
}


std::ostream &operator<<(std::ostream &os, const big_integer &value) {
  std::string tmp = std::to_string(value.digits.back());
  os << tmp;
  for (auto it = value.digits.rbegin() + 1; it != value.digits.rend(); it++) {
    tmp = std::to_string(*it);
    os << std::string(big_integer::base_log10 - tmp.length(), '0');
    os << tmp;
  }
  return os;
}
