#include <iostream>

#include "bitboard.hpp"

int main(int argc, char *argv[]) {

  const uint64_t lower_row = 0x00000000000000ff;

  uint pos = 0;
  std::cin >> pos;

  uint64_t K = 1ul << pos;

  auto positions = ~left_col << pos%8;
  positions |= (lower_row << (pos - pos%8));
  positions ^= K;

  std::cout << count_bits(positions) << "\n";
  std::cout << positions << "\n";

  // show_board(positions);
  return 0;
}
