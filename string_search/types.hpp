#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <string_view>
#include <vector>

using char_type = char;
using string_view_type = std::basic_string_view<char_type>;
using string_type = std::basic_string<char_type>;
using pos_type = string_view_type::size_type;
using diff_type = string_view_type::difference_type;

using pos_collection_type = std::vector<pos_type>;

#endif /* TYPES_HPP */
