#include <iostream>
#include <utility>
#include <cmath>

#include "utils.hpp"

double fibo_section(ulong n) {
  static const double phi = (1.0 + std::sqrt(5.0)) * 0.5;

  return power(phi, n) / std::sqrt(5.0) + 0.5;
}

big_integer fibo_iterative(ulong n) {
  big_integer f0 = 0;
  big_integer f1 = 1;
  big_integer tmp = f1;
  for (ulong i = 0; i < n; i++) {
    f1 += f0;
    f0 = tmp;
    tmp = f1;
  }

  return f0;
}

int main(int argc, char *argv[]) {
  ulong n = 0;
  std::cin >> n;

  // std::cout << fibo_iterative(n) << "\n";
  matrix22<big_integer> m{1u, 1u, 1u, 0u};
  // std::cout << m;
  if (n == 0)
    std::cout << 0 << "\n";
  else
    std::cout << power(m, n - 1, matrix22<big_integer>::eye())(0,0) << "\n";


  return 0;
}
