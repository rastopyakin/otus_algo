#ifndef UTF8_TOOLS_HPP
#define UTF8_TOOLS_HPP

#include <limits>
#include <string>
#include <vector>

namespace utf8 {

std::string random_text(size_t n_codes, u_int16_t min_code = 0x20,
                        u_int16_t max_code = std::numeric_limits<u_int16_t>::max());

std::vector<uint32_t> to_codes(std::string_view text);

} // namespace utf8

#endif /* UTF8_TOOLS_HPP */
