#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sys/types.h>

double power_iterative(double x, ulong n) {

  double result = 1.0;

  for (ulong i = 0; i < n; i++)
    result *= x;

  return result;
}

double power_log2(double x, ulong n) {

  double result = 1.0;
  double x_2 = x;

  while (n) {
    if (n % 2)
      result *= x_2;
    n /= 2;
    x_2 *= x_2;
  }

  return result;
}

double power_mult_rest(double x, ulong n) {
  double x2 = x;
  ulong x2_power = 1;

  if (n == 0) return 1.0;

// works for n > 0
  while (2*x2_power <= n) {
    x2 *= x2;
    x2_power *= 2;
  }

  return power_iterative(x, n - x2_power)*x2;
}

int main(int argc, char *argv[]) {
  double A = 0.0;
  ulong N = 0;

  std::cin >> A;

  std::cin >> N;

  std::cout << std::setprecision(12) << power_iterative(A, N) << "\n";

  return 0;
}
