#ifndef BM_SEARCH_HPP
#define BM_SEARCH_HPP

#include "types.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

template <class CharT> class BMSkipper {
public:
  template <class It> BMSkipper(It first, It last) {
    static_assert(std::is_trivial_v<CharT>);
    constexpr size_t table_size = 1 << sizeof(CharT) * 8;

    skip_table.reserve(table_size);
    skip_table.resize(table_size);
    pos_type size = std::distance(first, last);
    std::fill(skip_table.begin(), skip_table.end(), size);

    pos_type index = 0;
    std::advance(last, -1);
    for (It it = first; it != last; it++, index++) {
      skip_table[to_index(*it)] = size - 1 - index;
    }
  }

  pos_type skip(CharT ch) const { return skip_table[to_index(ch)]; }

private:
  constexpr pos_type to_index(CharT ch) const {
    static_assert(std::is_trivial_v<CharT>);
    return ch < 0 ? ch + (1 << 8 * sizeof(CharT)) : ch;
  }
  pos_collection_type skip_table{};
};

template <class RAIt_1, class RAIt_2,
          class Skipper = BMSkipper<typename std::iterator_traits<RAIt_2>::value_type>>
RAIt_1 boyer_moore_horspoole_search(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last) {

  if (p_first == p_last)
    return last;

  std::advance(first, std::distance(p_first, p_last) - 1);

  Skipper skipper{p_first, p_last};

  while (std::distance(first, last) > 0) {

    RAIt_1 t_it = first;
    RAIt_2 p_it = std::prev(p_last);
    while (p_it != p_first && *t_it == *p_it) {
      t_it--, p_it--;
    }

    if (p_it == p_first && *t_it == *p_it)
      return t_it;

    // keep going
    std::advance(first, skipper.skip(*first));
  }

  return last;
}

template <class RAIt_1, class RAIt_2, class OnFoundF,
          class Skipper = BMSkipper<typename std::iterator_traits<RAIt_2>::value_type>>
void boyer_moore_horspoole_search(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last,
                                  OnFoundF &&on_found_f) {

  static_assert(std::is_invocable_v<OnFoundF, RAIt_2>, "must accept iterator");

  if (p_first == p_last)
    return;

  std::advance(first, std::distance(p_first, p_last) - 1);

  Skipper skipper{p_first, p_last};

  while (std::distance(first, last) > 0) {

    RAIt_1 t_it = first;
    RAIt_2 p_it = std::prev(p_last);
    while (p_it != p_first && *t_it == *p_it) {
      t_it--, p_it--;
    }

    if (p_it == p_first && *t_it == *p_it)
      on_found_f(t_it);

    // keep going
    std::advance(first, skipper.skip(*first));
  }
}

pos_type boyer_moore_horspoole_search(std::string_view text, std::string_view pattern,
                                      pos_type start = 0);
// not implemented
pos_type boyer_moore_horspoole_search(std::u16string_view text, std::u16string_view pattern,
                                      pos_type start = 0);
pos_type boyer_moore_horspoole_search(std::u32string_view text, std::u32string_view pattern,
                                      pos_type start = 0);

pos_collection_type boyer_moore_horspoole_search_all(std::string_view text,
                                                     std::string_view pattern);
// not implemented
// pos_collection_type boyer_moore_horspoole_search_all(std::u16string_view text,
// std::u16string_view pattern);
// pos_collection_type boyer_moore_horspoole_search_all(std::u32string_view text,
// std::u32string_view pattern);

template <class RAIt, class Container> void Z_function(RAIt first, RAIt last, Container &cont) {

  cont[0] = 0; // Z(S, 0) == 0 by def

  pos_type value = 0;
  RAIt l = first, r = first;
  for (RAIt it = first + 1; it != last; it++) {
    if (std::distance(it, r) < 0)
      value = 0;
    else
      value = std::min(cont[std::distance(l, it)], static_cast<pos_type>(std::distance(it, r) + 1));

    while (first[value] == it[value] && it + value != last)
      value++;

    // TODO: fix comparison warning
    if (std::distance(it, r) > value - 1) {
      l = it;
      r = std::next(it, value - 1);
    }

    cont[std::distance(first, it)] = value;
  }
}

template <class RAIt, class Container> void suffix_table(RAIt first, RAIt last, Container &cont) {
  pos_type length = std::distance(first, last);
  // pos_collection_type cont(length + 1, length); // space for all suffixes including empty

  std::vector<size_t> rev_z(length);
  using RevIt = std::reverse_iterator<RAIt>;
  Z_function(RevIt{last}, RevIt{first}, rev_z);

  // check if there is pattern prefixes equal to some of its suffixes
  for (pos_type i = 1, j = 0; i < length; i++)
    if (rev_z[i] + i == length)
      for (; j <= i; j++) // set found suffix shift for smaller positions
        cont[j] = i;

  // fill shifts if there is full suffix in the middle of the pattern
  for (pos_type i = length - 1; i > 0; i--)
    cont[length - rev_z[i]] = i;
}

template <class RAIt_1, class RAIt_2,
          class Skipper = BMSkipper<typename std::iterator_traits<RAIt_2>::value_type>>
RAIt_1 boyer_moore_search(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last) {

  if (p_first == p_last)
    return last;

  size_t length = std::distance(p_first, p_last);
  std::advance(first, length - 1);

  Skipper skipper{p_first, p_last};
  std::vector<diff_type> suffix(length + 1, length);
  suffix_table(p_first, p_last, suffix);

  while (std::distance(first, last) > 0) {

    RAIt_1 t_it = first;
    RAIt_2 p_it = std::prev(p_last);
    while (p_it != p_first && *t_it == *p_it) {
      t_it--, p_it--;
    }

    if (p_it == p_first && *t_it == *p_it)
      return t_it;

    // keep going
    diff_type j = std::distance(p_first, p_it); // unmatched index in pattern
    diff_type m = skipper.skip(*t_it) + j + 1 - length;
    std::advance(first, std::max(m, suffix[j + 1]));
  }

  return last;
}

template <class RAIt_1, class RAIt_2, class OnFound,
          class Skipper = BMSkipper<typename std::iterator_traits<RAIt_2>::value_type>>
void boyer_moore_search(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last,
                        OnFound &&on_found) {

  if (p_first == p_last)
    return;

  size_t length = std::distance(p_first, p_last);
  std::advance(first, length - 1);

  Skipper skipper{p_first, p_last};
  std::vector<diff_type> suffix(length + 1, length);
  suffix_table(p_first, p_last, suffix);

  size_t bound = 0;
  while (std::distance(first, last) > 0) {

    RAIt_1 t_it = first;
    RAIt_2 p_it = std::prev(p_last);
    while (p_it != p_first + bound && *t_it == *p_it) {
      t_it--, p_it--;
    }

    if (p_it == p_first + bound && *t_it == *p_it) {
      t_it -= bound;
      on_found(t_it);
      bound = length - suffix[0];
      p_it = p_first;
    } else
      bound = 0;

    // keep going
    diff_type j = std::distance(p_first, p_it); // unmatched index in pattern
    diff_type m = skipper.skip(*t_it) + j + 1 - length;
    std::advance(first, std::max(m, suffix[j + 1]));
  }
}

pos_type boyer_moore_search(std::string_view text, std::string_view pattern, pos_type start = 0);
// not implemented
pos_type boyer_moore_search(std::u16string_view text, std::u16string_view pattern,
                            pos_type start = 0);
pos_type boyer_moore_search(std::u32string_view text, std::u32string_view pattern,
                            pos_type start = 0);

pos_collection_type boyer_moore_search_all(std::string_view text, std::string_view pattern);

namespace v0 {

pos_type boyer_moore_horspoole_search(string_view_type text, string_view_type pattern,
                                      pos_type start = 0);

pos_collection_type boyer_moore_horspoole_search_all(std::string_view text,
                                                     std::string_view pattern);

pos_type boyer_moore_search(string_view_type text, string_view_type pattern, pos_type start = 0);

pos_collection_type boyer_moore_search_all(std::string_view text, std::string_view pattern);

pos_collection_type Z_function_naive(string_view_type s);

pos_collection_type Z_function(string_view_type s);

pos_collection_type Z_to_pi(const std::vector<size_t> &z_values);

pos_collection_type suffix_shifts_naive(std::string_view pattern);

pos_collection_type suffix_shifts(string_view_type pattern);

} // namespace v0
#endif /* BM_SEARCH_HPP */
