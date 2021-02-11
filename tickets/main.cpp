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

int main(int argc, char *argv[]) {
  int N = 0;
  std::cin >> N;
  tickets_naive1(N, 0, 0);
  // std::cout << tickets_naive3() << "\n";
  std::cout << counter << "\n";


  return 0;
}
