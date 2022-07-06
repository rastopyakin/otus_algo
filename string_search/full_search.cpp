#include "full_search.hpp"
#include "types.hpp"

#include <iostream>
#include <iterator>
#include <string_view>

template <class CharT>
pos_type full_search(std::basic_string_view<CharT> text, std::basic_string_view<CharT> pattern,
                     pos_type start) {
  auto it = text.begin();
  std::advance(it, start);

  it = full_search(it, text.end(), pattern.begin(), pattern.end());

  return it != text.end() ? std::distance(text.begin(), it) : std::basic_string_view<CharT>::npos;
}

pos_type full_search(std::string_view text, std::string_view pattern, pos_type start) {
  return full_search<char>(text, pattern, start);
}

template <class CharT>
pos_collection_type full_search_all(std::basic_string_view<CharT> text,
                                    std::basic_string_view<CharT> pattern) {
  pos_collection_type found_positions{};
  auto start_it = text.begin();

  auto on_found = [&found_positions, &start_it](auto it) {
    found_positions.push_back(std::distance(start_it, it));
  };

  full_search(start_it, text.end(), pattern.begin(), pattern.end(), on_found);

  return found_positions;
}

pos_collection_type full_search_all(std::string_view text, std::string_view pattern) {
  return full_search_all<char>(text, pattern);
}

namespace v0 {

pos_type full_search(std::string_view text, std::string_view pattern, pos_type start) {

  if (pattern.empty())
    return std::string_view::npos;

  pos_type t_pos = start, p_pos = 0;

  while (t_pos + pattern.length() <= text.length()) {

    p_pos = 0;
    while (p_pos < pattern.length() && text[t_pos + p_pos] == pattern[p_pos])
      p_pos++;

    if (p_pos == pattern.length())
      return t_pos;

    // keep going
    t_pos++;
  }

  return std::string_view::npos;
}

pos_collection_type full_search_all(std::string_view text, std::string_view pattern) {
  pos_collection_type found_positions {};

  for (pos_type found_pos = full_search(text, pattern, 0);
       found_pos != std::string_view::npos;) {

    found_positions.push_back(found_pos);
    found_pos = full_search(text, pattern, found_pos + 1);
  }

  return found_positions;
}

} // namespace v0
