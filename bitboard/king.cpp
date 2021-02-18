#include <array>
#include <bits/stdint-uintn.h>
#include <iostream>

uint count_bits(uint64_t b) {

  uint counter = 0;
  while (b) {
    counter++;
    b &= b - 1;
  }
  return counter;
}

int main(int argc, char *argv[])
{
  uint pos = 0;
  std::cin >> pos;

  uint64_t K = 1ul << pos;

  const uint64_t left_row = ~0x101010101010101;
  const uint64_t right_row = ~0x8080808080808080;

  uint64_t positions =
    (right_row & (K << 7)) | (K << 8) | ((K << 9) & left_row) |
    (right_row & (K >> 1)) |            ((K << 1) & left_row) |
    (right_row & (K >> 9)) | (K >> 8) | ((K >> 7) & left_row);

  std::cout << count_bits(positions) << "\n";
  std::cout << positions << "\n";

  return 0;
}
