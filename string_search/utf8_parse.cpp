#include "utf8_tools.hpp"

#include <iostream>
#include <string>
#include <bitset>

int main(int argc, char *argv[]) {

  // auto str = random_utf8_string(5);
  // std::cout << str << "\n";

  std::string str;
  std::getline(std::cin, str);

  std::vector<std::bitset<8>> bitv;
  std::copy(str.begin(), str.end(), std::back_inserter(bitv));

  std::cout << "bytes: ";
  for (const auto &bs : bitv)
    std::cout << bs << " ";
  std::cout << "\n";

  std::vector<uint32_t> codes{utf8::to_codes(str)};

  std::cout << "UCS-2 codes: " << std::hex;
  for (uint32_t code : codes)
    std::cout << code << " ";
  std::cout << "\n";

  std::cout << std::dec << "\nTotal symbols: " << codes.size() << "\n";
  std::cout << "Total bytes: " << str.size() << "\n";

  return 0;
}
