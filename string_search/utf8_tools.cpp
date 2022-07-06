#include "utf8_tools.hpp"

#include <bitset>
#include <limits>
#include <random>

namespace utf8 {

std::string random_text(size_t n_codes, u_int16_t min_code, u_int16_t max_code) {

  std::uniform_int_distribution<uint16_t> d{min_code, max_code};
  std::random_device rd;
  std::mt19937 gen;
  gen.seed(rd());

  std::string str;

  auto is_surrogate = [](uint16_t code) { return 0xe000 <= code && code <= 0xf8ff; };
  auto is_private = [](uint16_t code) { return 0xd800 <= code && code <= 0xdfff; };

  uint16_t code = 0;

  for (size_t count = 0; count < n_codes;) {
    code = d(gen);
    if (0x20 <= code && code < 0x7f) {
      str.push_back(code);
      count++;
    } else if (0xa0 < code && code < 1 << 11) {
      char leading = 0b110 << 5 | code >> 6;
      str.push_back(leading);
      char trailing = 0b1 << 7 | (code & 0b0011'1111);
      str.push_back(trailing);
      count++;
    } else if (1 << 11 <= code && !is_private(code) && !is_surrogate(code)) {
      char leading = 0b1110 << 4 | code >> 12;
      str.push_back(leading);
      char trailing = 0b1 << 7 | (code >> 6 & 0b0011'1111);
      str.push_back(trailing);
      trailing = 0b1 << 7 | (code & 0b0011'1111);
      str.push_back(trailing);
      count++;
    }
  }

  return str;
}

std::vector<uint32_t> to_codes(std::string_view text) {

  // Final state machine parses the bytes. For simplicity the states below a stateless,
  // i.e. can't count bytes, so we need so many of them

  enum class State {
    ASCII,
    TwoBytes0,
    ThreeBytes0,
    ThreeBytes1,
    FourBytes0,
    FourBytes1,
    FourBytes2,
    Error
  };

  std::vector<uint32_t> codes;

  State state = State::ASCII;
  std::bitset<32> code;
  for (auto current = text.begin(); current != text.end();) {
    switch (state) {
    case State::ASCII:
      if (std::bitset<8> bs = *current; bs[7] == 0) {
        codes.push_back(*current);
        current++;
      } else if (bs[6] == 0) {
        state = State::Error;
        continue;
      } else if (bs[5] == 0) {
        code = (*current & 0b00011111) << 6;
        current++;
        state = State::TwoBytes0;
      } else if (bs[4] == 0) {
        code = (*current & 0b00001111) << 6;
        current++;
        state = State::ThreeBytes0;
      } else if (bs[3] == 0) {
        code = (*current & 0b00000111) << 6;
        current++;
        state = State::FourBytes0;
      } else {
        state = State::Error;
        continue;
      }
      break;

    case State::TwoBytes0:
      if (std::bitset<8> bs = *current; bs[7] && !bs[6]) {
        code |= *current & 0b00111111;
        codes.push_back(code.to_ulong());
        current++;
        state = State::ASCII;
      } else {
        state = State::Error;
      }

      break;
    case State::ThreeBytes0:
      if (std::bitset<8> bs = *current; bs[7] && !bs[6]) {
        code |= *current & 0b00111111;
        code <<= 6;
        current++;
        state = State::ThreeBytes1;
      } else
        state = State::Error;

      break;

    case State::ThreeBytes1:
      if (std::bitset<8> bs = *current; bs[7] && !bs[6]) {
        code |= *current & 0b00111111;
        codes.push_back(code.to_ulong());
        current++;
        state = State::ASCII;
      } else
        state = State::Error;
      break;

    case State::FourBytes0:
      if (std::bitset<8> bs = *current; bs[7] && !bs[6]) {
        code |= *current & 0b00111111;
        code <<= 6;
        current++;
        state = State::FourBytes1;
      } else
        state = State::Error;
      break;

    case State::FourBytes1:
      if (std::bitset<8> bs = *current; bs[7] && !bs[6]) {
        code |= *current & 0b00111111;
        code <<= 6;
        current++;
        state = State::FourBytes2;
      } else
        state = State::Error;
      break;

    case State::FourBytes2:
      if (std::bitset<8> bs = *current; bs[7] && !bs[6]) {
        code |= *current & 0b00111111;
        codes.push_back(code.to_ulong());
        current++;
        state = State::ASCII;
      } else
        state = State::Error;

      break;

    case State::Error:
      // TODO: return some error code
      break;
    }

    if (state == State::Error)
      break;
  }
  return codes;
}

} // namespace utf8
