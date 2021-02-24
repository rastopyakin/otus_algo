#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <bits/stdint-uintn.h>
#include <bitset>

const uint64_t left_col = ~0x101010101010101;
const uint64_t right_col = ~0x8080808080808080;

uint64_t bishop_moves(uint64_t board);

uint count_bits(uint64_t b);

void show_board(uint64_t board);

#endif /* BITBOARD_HPP */
