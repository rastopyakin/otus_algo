#include "kmp_search.hpp"

#include "types.hpp"

#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

template <class CharT>
pos_type kmp_search(std::basic_string_view<CharT> text, std::basic_string_view<CharT> pattern,
                    pos_type start) {

  std::string_view::iterator it =
      kmp_search_2(text.begin() + start, text.end(), pattern.begin(), pattern.end());
  return it != text.end() ? it - text.begin() : std::string_view::npos;
}

pos_type kmp_search(std::string_view text, std::string_view pattern, pos_type start) {
  return kmp_search<std::string_view::value_type>(text, pattern, start);
}

template <class CharT>
pos_collection_type kmp_search_all(std::basic_string_view<CharT> text,
                                   std::basic_string_view<CharT> pattern) {
  pos_collection_type found{};

  auto on_found = [&found, beg = text.begin()](auto it) { found.push_back(it - beg); };

  kmp_search_1(text.begin(), text.end(), pattern.begin(), pattern.end(), on_found);
  // kmp_search_2(text.begin(), text.end(), pattern.begin(), pattern.end(), on_found);

  return found;
}

pos_collection_type kmp_search_all(std::string_view text, std::string_view pattern) {
  return kmp_search_all<std::string_view::value_type>(text, pattern);
}

namespace v0 {
std::vector<size_t> pi_function_naive(string_view_type s) {
  std::vector<size_t> values;
  values.reserve(s.length());

  for (size_t i = 1; i <= s.length(); i++) {
    size_t value = i;

    long ind = value - 1;
    while (ind >= 0) {
      value--;
      ind = value - 1;
      while (s[ind] == s[i - value + ind] && ind >= 0) {
        ind--;
      }
    }

    values.push_back(value);
  }
  return values;
}

std::vector<size_t> pi_function(string_view_type s) {
  std::vector<size_t> values;
  values.reserve(s.length());
  values.push_back(0); // pi(s, 0) == 0

  size_t k = values.back();
  for (size_t i = 1; i < s.length(); i++) {

    while (s[k] != s[i] && k > 0)
      k = values[k - 1]; // k <- pi(s, k - 1)

    if (s[k] == s[i])
      k++;

    values.push_back(k);
  }
  return values;
}

pos_type kmp_search_1(string_view_type text, string_view_type pattern, pos_type start) {

  if (pattern.empty())
    return std::string_view::npos;

  auto pi{pi_function(pattern)};

  pos_type t_pos = start;
  pos_type p_pos = 0;

  pos_type counter = 0;

  while (t_pos < text.length()) {

    while (++counter && pattern[p_pos] == text[t_pos]) {
      p_pos++;
      t_pos++;
      if (p_pos == pattern.length())
        break;
    }

    if (p_pos == 0)
      t_pos++;
    else if (p_pos != pattern.length())
      p_pos = pi[p_pos - 1];
    else
      return t_pos - pattern.length();
  }
  // std::cout << "comparisons: " << counter << "\n";

  return string_type::npos;
}

pos_type kmp_search_2(string_view_type text, string_view_type pattern, pos_type start) {

  if (pattern.empty())
    return std::string_view::npos;

  auto pi{pi_function(pattern)};

  pos_type t_pos = start;
  pos_type p_pos = 0;

  pos_type counter = 0;

  for (; t_pos < text.length(); t_pos++) {

    while (p_pos > 0 && ++counter && pattern[p_pos] != text[t_pos])
      p_pos = pi[p_pos - 1];

    if (++counter && text[t_pos] == pattern[p_pos])
      p_pos++;

    if (p_pos == pattern.length())
      return t_pos - pattern.length() + 1;
  }

  // std::cout << "comparisons: " << counter << "\n";

  return string_view_type::npos;
}

pos_collection_type kmp_search_all_1(string_view_type text, string_view_type pattern) {

  pos_collection_type found_positions{};

  if (pattern.empty())
    return found_positions;

  auto prefix{pi_function(pattern)};

  pos_type t_pos = 0;
  pos_type p_pos = 0;

  while (t_pos < text.length()) {

    while (pattern[p_pos] == text[t_pos]) {
      p_pos++, t_pos++;
      if (p_pos == pattern.length()) // checking this inside the loop is faster in average
        break;
    }

    // wow.. somehow the way this part is written influences the performance of the NEXT function
    // kmp_search_2; at least in G++ 11. Clang behaves normal. WTF?
    if (p_pos == 0)
      t_pos++;
    else {
      if (p_pos == pattern.length())
        found_positions.push_back(t_pos - pattern.length());
      p_pos = prefix[p_pos - 1];
    }
  }

  return found_positions;
}

// On random strings this function work significantly faster (about 15%) than previous one though it
// obviously can make twice more comparisons. Some strange optimizer behaviour (g++ 11)?
// It's basic version (see .hpp) is slower as it actually should be according to the intuitive
// impression
pos_collection_type kmp_search_all_2(string_view_type text, string_view_type pattern) {

  pos_collection_type found_positions{};

  if (pattern.empty())
    return found_positions;

  auto prefix{pi_function(pattern)};

  pos_type t_pos = 0;
  pos_type p_pos = 0;

  for (; t_pos < text.length(); t_pos++) {

    while (p_pos > 0 && pattern[p_pos] != text[t_pos])
      p_pos = prefix[p_pos - 1];

    if (text[t_pos] == pattern[p_pos])
      p_pos++;

    if (p_pos == pattern.length()) {
      found_positions.push_back(t_pos - pattern.length() + 1);
      p_pos = prefix[p_pos - 1];
    }
  }

  return found_positions;
}

} // namespace v0
