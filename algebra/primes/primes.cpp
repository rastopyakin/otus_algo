#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

using prime_predicate_t = bool (*)(uint64_t);

bool is_prime_naive(uint64_t n) {
  for (uint64_t i = 2; i < n; i++)
    if (n % i == 0)
      return false;
  return true;
  ;
}

bool is_prime_sqrt(uint64_t n) {
  uint64_t p = std::sqrt(n);
  for (uint64_t i = 2; i <= p; i++)
    if (n % i == 0)
      return false;
  return true;
  ;
}

// do not call this with arbitrary n,
// first it must be called for all 2...(n-1)
static std::vector<uint64_t> primes_found{2};
bool is_prime_array(uint64_t n) {

  for (size_t i = 1; i < primes_found.size(); i++)
    if (n % primes_found[i] == 0)
      return false;

  primes_found.push_back(n);
  return true;
}

uint64_t count_primes(uint64_t N, prime_predicate_t is_prime) {
  uint64_t counter = 0;
  for (uint64_t i = 2; i <= N; i++) {
    if (is_prime(i))
      counter++;
  }
  return counter;
}

uint64_t eratosthenes_count(uint64_t n) {
  std::vector<bool> is_prime(n + 1, true);
  uint64_t counter = 0;

  for (size_t i = 2; i <= n; i++)
    if (is_prime[i]) {
      counter++;
      for (size_t p = i * i; p <= n; p += i)
        is_prime[p] = false;
    }

  return counter;
}

int main(int argc, char *argv[]) {
  uint64_t N;
  std::cin >> N;
  // std::array<uint64_t, 100000> primes;
  // std::cout << count_primes(N, is_prime_naive) << "\n";
  // std::cout << count_primes(N, is_prime_sqrt) << "\n";
  // primes_found.reserve(100000);
  std::cout << count_primes(N, is_prime_array) << "\n";
  // std::cout << eratosthenes_count(N) << "\n";

  for (auto prime : primes_found)
    std::cout << prime << ", ";
  std::cout << "\n";

  constexpr std::size_t max_num = std::numeric_limits<std::size_t>::max();
  std::cout << "max_num: " << max_num << "\n";
  return 0;
}
