#include "utf8_tools.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {

  size_t n_codes = 0;

  if (argc != 2) {
    std::cerr << "usage: unigen [number of code points to generate]\n";
    std::exit(1);
  } else
    n_codes = std::strtoul(argv[1], nullptr, 10);


  auto str = utf8::random_text(n_codes);
  std::cout << str << "\n";

  std::vector<uint32_t> codes{utf8::to_codes(str)};

  std::cout << "UCS-2 codes: " << std::hex;
  for (uint32_t code : codes)
    std::cout << code << " ";
  std::cout << "\n";

  std::cout << std::dec << "\nTotal symbols: " << codes.size() << "\n";
  std::cout << "Total bytes: " << str.size() << "\n";

  return 0;
}
