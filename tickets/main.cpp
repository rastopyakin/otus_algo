#include <iostream>

uint64_t counter = 0;

void tickets_naive1(int N, int s1, int s2) {

  if (N == 0) {
    if (s1 == s2)
      counter++;
    return;
  }
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
      tickets_naive1(N - 1, s1 + i, s2 + j);
}

uint64_t count(int n_digits, int sum) {
  uint64_t counter = 0;
  if (n_digits == 1)
      return 1;

  int n_digits_left = n_digits/2;
  int n_digits_right = n_digits - n_digits_left;
  int s_left_min = sum >= n_digits_right*9  ? sum - n_digits_right*9 : 0;
  int s_left_max = sum >= n_digits_left*9 ? n_digits_left*9 : sum;
  for (int s1 = s_left_min; s1 <= s_left_max; s1++)
    counter += count(n_digits_left, s1)*count(n_digits_right, sum - s1);

  return counter;
}

int main(int argc, char *argv[]) {
  int N = 0;
  std::cin >> N;

  uint64_t counter = 0;

  for (int s = 0; s <= 9*N; s++) {
    auto ss = count(N, s);
    counter += ss*ss;
  }

  std::cout << counter;
  return 0;
}
