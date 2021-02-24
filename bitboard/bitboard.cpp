#include <iostream>

#include "bitboard.hpp"

uint64_t bishop_moves(uint64_t board) {

  uint64_t positions = 0;
  uint64_t temp = board;

  while ((temp &= right_col) and (temp <<= 9))
    positions |= temp;

  temp = board;
  while ((temp &= right_col) and (temp >>= 7))
    positions |= temp;

  temp = board;
  while ((temp &= left_col) and (temp >>= 9))
    positions |= temp;

  temp = board;
  while ((temp &= left_col) and (temp <<= 7))
    positions |= temp;

  return positions;
}

uint count_bits(uint64_t b) {

  uint counter = 0;
  while (b) {
    counter++;
    b &= b - 1;
  }
  return counter;
}

void show_board(uint64_t board) {

  std::bitset<64> b = board;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int pos = 56 - 8 * i + j;
      std::cout << b.test(pos) << ' ';
    }
    std::cout << "\n";
  }
}
