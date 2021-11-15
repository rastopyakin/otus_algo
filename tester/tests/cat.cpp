#include <iostream>

int main(int argc, char *argv[]) {
  for (char c; std::cin.get(c);)
    std::cout.put(c);

  return 0;
}
