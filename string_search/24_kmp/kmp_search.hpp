#ifndef KMP_SEARCH_HPP
#define KMP_SEARCH_HPP

#include "types.hpp"

#include <cstddef>
#include <iterator>
#include <string_view>
#include <type_traits>
#include <vector>

template <class RAIt, class Container> void pi_function(RAIt first, RAIt last, Container &cont) {
  cont[0] = 0; // pi(S, 0) == 0

  size_t length = std::distance(first, last);
  size_t k = 0;
  for (size_t i = 1; i < length; i++) {

    while (first[k] != first[i] && k > 0)
      k = cont[k - 1];

    if (first[i] == first[k])
      k++;

    cont[i] = k;
  }
}

template <class RAIt_1, class RAIt_2>
RAIt_2 kmp_search_1(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last) {
  if (p_first == p_last)
    return last;

  size_t length = std::distance(p_first, p_last);
  std::vector<size_t> prefix(length);
  pi_function(p_first, p_last, prefix);

  RAIt_2 p_it = p_first;

  while (first != last) {
    while (*p_it == *first) {
      p_it++, first++;
      if (p_it == p_last)
        break;
    }

    if (p_it == p_first)
      first++;
    else if (p_it != p_last)
      p_it = std::next(p_first, prefix[std::distance(p_first, p_it) - 1]);
    else
      return first - length;
  }
  return last;
}

template <class RAIt_1, class RAIt_2>
RAIt_2 kmp_search_2(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last) {
  if (p_first == p_last)
    return last;

  size_t length = std::distance(p_first, p_last);
  std::vector<size_t> prefix(length);
  pi_function(p_first, p_last, prefix);

  RAIt_2 p_it = p_first;

  for (; first != last; first++) {
    while (p_it != p_first && *p_it != *first)
      p_it = p_first + prefix[std::distance(p_first, p_it) - 1];

    if (*p_it == *first)
      p_it++;

    if (p_it == p_last)
      return first - length + 1;
  }
  return last;
}

pos_type kmp_search(std::string_view text, std::string_view pattern, pos_type start = 0);
// not implemented
pos_type kmp_search(std::u16string_view text, std::u16string_view pattern, pos_type start = 0);
pos_type kmp_search(std::u32string_view text, std::u32string_view pattern, pos_type start = 0);

template <class RAIt_1, class RAIt_2, class OnFoundFunc>
void kmp_search_1(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last,
                  OnFoundFunc &&on_found) {

  static_assert(std::is_invocable_v<OnFoundFunc, RAIt_2>, "should accept pattern iterator");

  if (p_first == p_last)
    return;

  size_t length = std::distance(p_first, p_last);
  std::vector<size_t> prefix(length);
  pi_function(p_first, p_last, prefix);

  RAIt_2 p_k = p_first;
  while (first != last) {
    while (*p_k == *first) {
      p_k++, first++;
      if (p_k == p_last) // way faster than comparing in the loop condition
        break;
    }

    if (p_k == p_first)
      first++;
    else {
      if (p_k == p_last)
        on_found(first - length);
      p_k = p_first + prefix[std::distance(p_first, p_k) - 1];
    }
  }
}

template <class RAIt_1, class RAIt_2, class OnFoundFunc>
void kmp_search_2(RAIt_1 first, RAIt_1 last, RAIt_2 p_first, RAIt_2 p_last, OnFoundFunc &&on_found) {
  if (p_first == p_last)
    return;

  size_t length = std::distance(p_first, p_last);
  std::vector<size_t> prefix(length);
  pi_function(p_first, p_last, prefix);

  RAIt_2 p_it = p_first;

  for (; first != last; first++) {

    while (*p_it != *first && p_it != p_first) {
      p_it = p_first + prefix[std::distance(p_first, p_it) - 1];
    }

    if (*p_it == *first)
      p_it++;

    if (p_it == p_last) {
      on_found(first - length + 1);
      p_it = p_first + prefix[length - 1];
    }
  }
}

pos_collection_type kmp_search_all(std::string_view text, std::string_view pattern);
// not implemented
// pos_type kmp_search_all(std::u16string_view text, std::u16string_view pattern);
// pos_type kmp_search_all(std::u32string_view text, std::u32string_view pattern);

namespace v0 {
pos_collection_type pi_function_naive(string_view_type s);
pos_collection_type pi_function(string_view_type s);

pos_type kmp_search_1(string_view_type text, string_view_type pattern, pos_type start = 0);

pos_type kmp_search_2(string_view_type text, string_view_type pattern, pos_type start = 0);

pos_collection_type kmp_search_all_1(std::string_view text, std::string_view pattern);
pos_collection_type kmp_search_all_2(std::string_view text, std::string_view pattern);

} // namespace v0

#endif /* KMP_SEARCH_HPP */
