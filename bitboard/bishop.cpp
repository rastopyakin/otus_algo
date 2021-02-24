#include <iostream>

#include "bitboard.hpp"

uint64_t bishop_moves_2(uint64_t board) {

  uint64_t factor = 0;
  uint64_t mult = 128;

  for (int i = 0; i < 7; i++) {
    factor += mult;
    mult *= 128;
  }

  const uint64_t main_diag = 0x8040201008040200;

  // main diagonal up
  uint64_t positions = ((board*main_diag) & 0x80c0e0f0f8fcfeff) | (board&0x7f3f1f0f07030100)*main_diag;
  // secondary diagonal up
  positions |= ((board*factor) & 0x103070f1f3f7f) | (board&0xfffefcf8f0e0c080)*factor;

  // for down moves we need division which is expensive

  return positions;
}

int main(int argc, char *argv[]) {
  uint pos = 0;
  std::cin >> pos;

  uint64_t K = 1ul << pos;

  auto positions = bishop_moves(K);

  std::cout << count_bits(positions) << "\n";
  std::cout << positions;

  std::cout << "\n";

  return 0;
}
