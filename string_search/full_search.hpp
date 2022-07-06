#ifndef FULL_SEARCH_HPP
#define FULL_SEARCH_HPP

#include "types.hpp"
#include <cstddef>
#include <iterator>
#include <string_view>
#include <type_traits>
#include <vector>

template <class FwdIt_1, class FwdIt_2>
FwdIt_1 full_search(FwdIt_1 first, FwdIt_1 last, FwdIt_2 p_first, FwdIt_2 p_last) {

  if (p_first == p_last)
    return last;

  using difference_type = typename std::iterator_traits<FwdIt_2>::difference_type;

  difference_type length = std::distance(p_first, p_last);
  while (std::distance(first, last) >= length) {

    FwdIt_1 t_it = first;
    FwdIt_2 p_it = p_first;

    while (*p_it == *t_it && p_it != p_last && t_it != last)
      p_it++, t_it++;

    if (p_it == p_last)
      return first;

    first++;
  }

  return last;
}

template <class FwdIt_1, class FwdIt_2, class OnFoundF>
void full_search(FwdIt_1 first, FwdIt_1 last, FwdIt_2 p_first, FwdIt_2 p_last,
                 OnFoundF &&on_found_f) {

  static_assert(std::is_invocable_v<OnFoundF, FwdIt_2>, "must accept iterator");

  for (FwdIt_1 it = full_search(first, last, p_first, p_last); it != last;) {
    on_found_f(it);
    it = full_search(++it, last, p_first, p_last);
  }
}

pos_type full_search(std::string_view text, std::string_view pattern, pos_type start = 0);

pos_collection_type full_search_all(std::string_view text, std::string_view pattern);

// not implemented
pos_type full_search(std::u16string_view text, std::u16string_view pattern, pos_type start = 0);
pos_type full_search(std::u32string_view text, std::u32string_view pattern, pos_type start = 0);

namespace v0 {
pos_type full_search(std::string_view text, std::string_view pattern, pos_type start = 0);

pos_collection_type full_search_all(std::string_view text, std::string_view pattern);
} // namespace v0

#endif /* FULL_SEARCH_HPP */
