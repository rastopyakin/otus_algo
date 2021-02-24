#include <iostream>

#include "bitboard.hpp"

uint64_t castle_moves(uint64_t board) {

  uint64_t positions = 0;
  uint64_t temp = board;
  for (int i = 1; i < 8; i++) {
    temp &= right_col;
    temp <<= 1;
    positions |= temp;
  }

  temp = board;
  for (int i = 1; i < 8; i++) {
    temp &= left_col;
    temp >>= 1;
    positions |= temp;
  }

  for (int i = 1; i < 8; i++) {
    positions |= ((board << 8*i) | (board >> 8*i));
  }

  return positions;
}

int main(int argc, char *argv[])
{
  uint pos = 0;
  std::cin >> pos;

  uint64_t K = 1ul << pos;

  uint64_t positions = bishop_moves(K) | castle_moves(K);

  std::cout << count_bits(positions) << "\n";
  std::cout << positions;

  // std::cout << "\n";
  // show_board(positions);

  return 0;
}
