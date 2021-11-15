#include <chrono>
#include <cstdint>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

int main(int argc, char *argv[]) {


  uint64_t result = 0;
  int counter = 0;
  for (uint64_t input; std::cin >> input;) {
    // std::cout << input << " ";
    counter++;
    result += input;
  }
  // std::cout << std::endl;
  std::cout << "counter : " << counter << "\n";
  std::cout << result << "\n";
  return 0;
}
