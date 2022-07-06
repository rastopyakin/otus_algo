#include "bm_search.hpp"
#include "types.hpp"

#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

template <class CharT>
pos_type boyer_moore_horspoole_search(std::basic_string_view<CharT> text,
                                      std::basic_string_view<CharT> pattern, pos_type start) {
  auto it = text.begin();
  std::advance(it, start);

  it = boyer_moore_horspoole_search(it, text.end(), pattern.begin(), pattern.end());

  return it != text.end() ? std::distance(text.begin(), it) : std::basic_string_view<CharT>::npos;
}

pos_type boyer_moore_horspoole_search(std::string_view text, std::string_view pattern,
                                      pos_type start) {
  return boyer_moore_horspoole_search<char>(text, pattern, start);
}

template <class CharT>
pos_collection_type boyer_moore_horspoole_search_all(std::basic_string_view<CharT> text,
                                                     std::basic_string_view<CharT> pattern) {
  pos_collection_type found_positions{};
  auto start_it = text.begin();

  auto on_found = [&found_positions, start_it = text.begin()](auto it) {
    found_positions.push_back(std::distance(start_it, it));
  };

  boyer_moore_horspoole_search(start_it, text.end(), pattern.begin(), pattern.end(), on_found);

  return found_positions;
}

pos_collection_type boyer_moore_horspoole_search_all(std::string_view text,
                                                     std::string_view pattern) {
  return boyer_moore_horspoole_search_all<char>(text, pattern);
}

template <class CharT>
pos_type boyer_moore_search(std::basic_string_view<CharT> text,
                            std::basic_string_view<CharT> pattern, pos_type start) {
  auto it = text.begin();
  std::advance(it, start);

  it = boyer_moore_search(it, text.end(), pattern.begin(), pattern.end());

  return it != text.end() ? std::distance(text.begin(), it) : std::basic_string_view<CharT>::npos;
}

pos_type boyer_moore_search(std::string_view text, std::string_view pattern, pos_type start) {
  return boyer_moore_search<char>(text, pattern, start);
}

template <class CharT>
pos_collection_type boyer_moore_search_all(std::basic_string_view<CharT> text,
                                           std::basic_string_view<CharT> pattern) {
  pos_collection_type found_positions{};
  auto start_it = text.begin();

  auto on_found = [&found_positions, start_it = text.begin()](auto it) {
    found_positions.push_back(std::distance(start_it, it));
  };

  boyer_moore_search(start_it, text.end(), pattern.begin(), pattern.end(), on_found);

  return found_positions;
}

pos_collection_type boyer_moore_search_all(std::string_view text, std::string_view pattern) {
  return boyer_moore_search_all<char>(text, pattern);
}

namespace v0 {

template <class CharT> pos_type constexpr to_index(CharT ch) {
  static_assert(std::is_trivial_v<CharT>);
  return ch < 0 ? ch + (1 << 8 * sizeof(CharT)) : ch;
}

pos_collection_type bad_char_shifts(std::string_view pattern) {
  pos_collection_type shifts;
  constexpr size_t table_size = 1 << sizeof(char) * 8;
  shifts.reserve(table_size);
  shifts.resize(table_size);
  std::fill(shifts.begin(), shifts.end(), pattern.length());
  for (pos_type i = 0; i < pattern.length() - 1; i++)
    shifts[to_index(pattern[i])] = pattern.length() - i - 1;

  return shifts;
}

pos_type boyer_moore_horspoole_search(std::string_view text, std::string_view pattern,
                                      pos_type start) {

  auto shifts = bad_char_shifts(pattern);

  pos_type t_pos = start;
  pos_type p_pos = pattern.length();

  while (p_pos > 0 && t_pos <= text.length() - pattern.length()) {
    p_pos = pattern.length(); // as p_pos is unsigned we add 1 to its value so that the
                              // search is finished when it reaches 0
    while (p_pos > 0 && text[t_pos + p_pos - 1] == pattern[p_pos - 1])
      p_pos--;

    if (p_pos) {
      auto last_char = text[t_pos + pattern.length() - 1];
      t_pos += shifts[to_index(last_char)];
    }
  }

  return p_pos == 0 ? t_pos : string_type::npos;
}

pos_collection_type boyer_moore_horspoole_search_all(std::string_view text,
                                                     std::string_view pattern) {

  pos_collection_type found_positions;

  auto shifts = bad_char_shifts(pattern);

  pos_type t_pos = 0;
  pos_type p_pos = pattern.length();

  while (t_pos <= text.length() - pattern.length()) {
    p_pos = pattern.length(); // as p_pos is unsigned we add 1 to its value so that the
                              // search is finished when it reaches 0
    while (p_pos > 0 && text[t_pos + p_pos - 1] == pattern[p_pos - 1])
      p_pos--;

    if (!p_pos)
      found_positions.push_back(t_pos);

    auto last_char = text[t_pos + pattern.length() - 1];
    t_pos += shifts[to_index(last_char)];
  }

  return found_positions;
}

pos_collection_type suffix_shifts_naive(std::string_view pattern) {
  pos_collection_type shifts;
  shifts.reserve(pattern.length() + 1);

  // TODO: fix comparison warning
  for (diff_type pos = 0; pos < pattern.length() + 1; pos++) {
    // pattern[pos] is first suffix character
    diff_type candidate = 1;
    auto suffix = pattern.substr(pos, pattern.length() - pos);

    while (pos - 1 - candidate >= 0) {
      if (pattern[pos - 1 - candidate] != pattern[pos - 1] &&
          pattern.substr(pos - candidate, suffix.length()) == suffix) {
        shifts.push_back(candidate);
        break;
      }
      candidate++;
    }

    if (pos - 1 - candidate < 0) {
      // maybe there is some suffix of the suffix equal to the pattern prefix

      // the case when the suffix is the whole pattern is special by the same reason as Z(S, 0)
      // actually shifts[0] == shifts[1] always, there is no difference between them
      pos_type prefix_len =
          suffix.length() == pattern.length() ? pattern.length() - 1 : suffix.length();

      auto prefix = pattern.substr(0, prefix_len);

      while (prefix_len > 0 && prefix != suffix.substr(suffix.length() - prefix_len, prefix_len)) {
        prefix_len--;
        prefix = pattern.substr(0, prefix_len);
      }

      if (prefix_len)
        shifts.push_back(pattern.length() - prefix_len);
      else
        shifts.push_back(pattern.length());
    }
  }

  return shifts;
}

pos_collection_type suffix_shifts(std::string_view pattern) {
  pos_collection_type shifts;
  shifts.reserve(pattern.length() + 1);
  shifts.resize(pattern.length() + 1, pattern.length());

  // shifts.resize(pattern.length() + 1, 0);

  string_type reversed;
  std::reverse_copy(pattern.begin(), pattern.end(), std::back_inserter(reversed));

  auto z_rvs = Z_function(reversed);

  // check if there is pattern prefix equal to some of its suffix
  for (pos_type i = 1, j = 0; i < pattern.length(); i++)
    if (z_rvs[i] + i == pattern.length())
      for (; j <= i; j++)
        shifts[j] = i;

  // fill shifts if there is full suffix in the middle of the pattern
  for (pos_type i = pattern.length() - 1; i > 0; i--)
    shifts[pattern.length() - z_rvs[i]] = i;

  return shifts;
}

pos_type boyer_moore_search(string_view_type text, string_view_type pattern, pos_type start) {

  if (!pattern.length())
    return std::string_view::npos;

  pos_type t_pos = start;
  pos_type p_pos = pattern.length();

  auto bc_shifts = bad_char_shifts(pattern);
  auto suff_shifts = suffix_shifts(pattern);

  while (p_pos > 0 && t_pos <= text.length() - pattern.length()) {
    p_pos = pattern.length(); // as p_pos is unsigned we add 1 to its value so that the
                              // search is finished when it reaches 0
    while (p_pos > 0 && text[t_pos + p_pos - 1] == pattern[p_pos - 1])
      p_pos--;

    if (p_pos) {
      auto bad_char = text[t_pos + p_pos - 1];
      diff_type t_badchar = bc_shifts[to_index(bad_char)] + p_pos - pattern.length();
      diff_type t_suff = suff_shifts[p_pos];

      t_pos += std::max(t_badchar, t_suff);
      // t_pos += suff_shifts[p_pos];
    }
  }

  return p_pos == 0 ? t_pos : string_type::npos;
}

pos_collection_type boyer_moore_search_all(std::string_view text, std::string_view pattern) {

  pos_collection_type found_positions;

  if (!pattern.length())
    return found_positions;

  pos_type t_pos = 0;
  pos_type p_pos = pattern.length();
  pos_type bound = 0;

  auto bc_shifts = bad_char_shifts(pattern);
  auto suff_shifts = suffix_shifts(pattern);

  while (t_pos <= text.length() - pattern.length()) {
    p_pos = pattern.length(); // as p_pos is unsigned we add 1 to its value so that the
                              // search is finished when it reaches 0

    // pattern[bound - 1 ... 0] is already proven to match by previous checks
    while (p_pos > bound && text[t_pos + p_pos - 1] == pattern[p_pos - 1])
      p_pos--;
    // at this point pattern[p_pos] is the last matched character

    if (p_pos == bound) {
      found_positions.push_back(t_pos);
      bound = pattern.length() - suff_shifts[0];
      p_pos = 0;
    } else
      bound = 0; // the shift is not known here and it's too complex to predict new bound
                 // so cancel the optimization to prevent false pattern occurrence

    auto bad_char = text[t_pos + p_pos - 1];
    diff_type t_badchar = bc_shifts[to_index(bad_char)] + p_pos - pattern.length();

    diff_type t_suff = suff_shifts[p_pos];

    t_pos += std::max(t_badchar, t_suff);
    // t_pos += suff_shifts[p_pos];
  }

  return found_positions;
}

std::vector<size_t> Z_function_naive(string_view_type s) {

  std::vector<size_t> values;
  values.reserve(s.length());
  values.push_back(0); // Z(S, 0) == 0

  for (size_t pos = 1; pos < s.length(); pos++) {
    size_t value = 0;
    while (s[value] == s[pos + value] && pos + value < s.length())
      value++;
    values.push_back(value);
  }

  return values;
}

std::vector<size_t> Z_function(string_view_type s) {
  std::vector<size_t> values;
  values.reserve(s.length());
  values.push_back(0); // Z(S, 0) == 0

  size_t l = 0, r = 0;
  size_t value = 0;
  for (size_t pos = 1; pos < s.length(); pos++) {
    if (pos > r)
      value = 0;
    else
      value = std::min(values[pos - l], r - pos + 1);

    while (s[value] == s[pos + value] && pos + value < s.length()) {
      value++;
    }

    if (pos + value - 1 > r) {
      l = pos;
      r = pos + value - 1;
    }

    values.push_back(value);
  }

  return values;
}

std::vector<size_t> Z_to_pi(const std::vector<size_t> &z_values) {
  // TODO: understand this
  std::vector<size_t> values;
  values.resize(z_values.size());

  for (size_t i = 1; i < z_values.size(); i++)
    if (z_values[i])
      for (int j = z_values[i] - 1; j >= 0 && !(values[i + j]); j--)
        values[i + j] = j + 1;

  return values;
}

} // namespace v0
