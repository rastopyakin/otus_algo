#include <iostream>
#include <cmath>

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

class sums {
public:
  uint64_t count(int n_digits, int sum) {
    counter = 0;
    number_impl(n_digits, sum, 0);
    return counter;
  }

private:
  uint64_t counter;
  void number_impl(int N, const int &s, int s1) {

    if (N == 0) {
      if (s1 == s) {
        counter++;
      }
      return;
    }
    for (int i = 0; i < 10; i++)
      number_impl(N-1, s, s1+i);
  }
};



int main(int argc, char *argv[]) {
  int N = 0;
  std::cin >> N;
  // tickets_naive1(N, 0, 0);
  // std::cout << tickets_naive3() << "\n";
  // std::cout << counter << "\n";

  uint64_t counter = 0;
  sums n_sum;
  for (int s = 0; s <= 9*N; s++) {
    auto ss = n_sum.count(N, s);
    counter += ss*ss;
  }
  std::cout << counter << "\n";
  return 0;
}
