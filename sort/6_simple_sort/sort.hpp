#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <class BDIterator> struct bubble_sort_v1 {

  bubble_sort_v1(BDIterator first, BDIterator last) {

    using dist_type = typename std::iterator_traits<BDIterator>::difference_type;
    dist_type size = std::distance(first, last);

    for (dist_type i = 0; i < size; i++) {
      BDIterator sorted_border = std::next(last, -i - 1);
      for (BDIterator j = first; j != sorted_border; std::advance(j, 1)) {
        if (*j > *std::next(j))
          std::swap(*j, *std::next(j));
      }
    }
  }
};

template <class BDIterator> struct bubble_sort_v2 {
  bubble_sort_v2(BDIterator first, BDIterator last) {

    using dist_type = typename std::iterator_traits<BDIterator>::difference_type;
    dist_type size = std::distance(first, last);

    bool stop_flag = true;

    for (dist_type i = 0; i < size; i++) {
      BDIterator sorted_border = std::next(last, -i - 1);
      stop_flag = true;
      for (BDIterator j = first; j != sorted_border; std::advance(j, 1)) {
        if (*j > *std::next(j)) {
          std::swap(*j, *std::next(j));
          stop_flag = false;
        }
      }
      if (stop_flag)
        break;
    }
  }
};

template <class BDIterator> struct insertion_sort_v1 {
  insertion_sort_v1(BDIterator first, BDIterator last) {

    using dist_type = typename std::iterator_traits<BDIterator>::difference_type;
    dist_type size = std::distance(first, last);

    for (dist_type first_unsorted = 1; first_unsorted < size; first_unsorted++) {
      BDIterator it = std::next(first, first_unsorted);

      while (it != first && *it < *std::prev(it)) {
        std::swap(*it, *std::prev(it));
        std::advance(it, -1);
      }
    }
  }
};

template <class BDIterator> struct insertion_sort_v1_5 {
  insertion_sort_v1_5(BDIterator first, BDIterator last) {

    using dist_type = typename std::iterator_traits<BDIterator>::difference_type;
    using T = typename std::iterator_traits<BDIterator>::value_type;
    dist_type size = std::distance(first, last);

    for (dist_type first_unsorted = 1; first_unsorted < size; first_unsorted++) {
      BDIterator it = std::next(first, first_unsorted);

      T elem = std::move(*it);

      while (it != first && elem < *std::prev(it)) {
        *it = std::move(*std::prev(it));
        std::advance(it, -1);
      }

      *it = std::move(elem);
    }
  }
};

template <class T, class BDIterator>
BDIterator upper_bound_linear(const T &value, BDIterator first, BDIterator last) {
  while (last != first && value < *std::prev(last))
    std::advance(last, -1);

  return last;
}

template <class BDIterator> struct insertion_sort_v2 {
  insertion_sort_v2(BDIterator first, BDIterator last) {

    using dist_type = typename std::iterator_traits<BDIterator>::difference_type;
    using T = typename std::iterator_traits<BDIterator>::value_type;
    dist_type size = std::distance(first, last);

    for (dist_type first_unsorted = 1; first_unsorted < size; first_unsorted++) {
      BDIterator fu_it = std::next(first, first_unsorted);

      BDIterator it = upper_bound_linear(*fu_it, first, fu_it);

      T elem = std::move(*fu_it);
      std::move_backward(it, fu_it, std::next(fu_it));
      *it = std::move(elem);
    }
  }
};

template <class T, class BDIterator>
BDIterator upper_bound_log(const T &value, BDIterator first, BDIterator last) {
  static_assert(std::is_same_v<T, typename std::iterator_traits<BDIterator>::value_type>);
  using dist_type = typename std::iterator_traits<BDIterator>::difference_type;

  dist_type length = std::distance(first, last);
  BDIterator middle = first;

  while (length > 0) {
    std::advance(middle, length / 2);

    if (value >= *middle) {
      std::advance(middle, 1);
      first = middle;
      length -= length / 2 + 1;
    } else {
      length /= 2;
      middle = first;
    }
  }

  return middle;
}

template <class BDIterator> struct insertion_sort_v3 {
  insertion_sort_v3(BDIterator first, BDIterator last) {

    using dist_type = typename std::iterator_traits<BDIterator>::difference_type;
    using T = typename std::iterator_traits<BDIterator>::value_type;
    dist_type size = std::distance(first, last);

    // int v_pos = std::to_string(*first).length() + 1;

    for (dist_type first_unsorted = 1; first_unsorted < size; first_unsorted++) {

      // std::cout << "first unsorted : " << first_unsorted << "\n";
      // std::cout << std::string(v_pos, ' ') << "v\n";
      // std::copy(first, last, std::ostream_iterator<T>{std::cout, " "});
      // std::cout << "\n";

      BDIterator fu_it = std::next(first, first_unsorted);
      // v_pos += std::to_string(*fu_it).length() + 1;

      BDIterator it = upper_bound_log(*fu_it, first, fu_it);

      T elem = std::move(*fu_it);
      std::move_backward(it, fu_it, std::next(fu_it));
      *it = std::move(elem);
    }
  }
};

template <class BDIterator, class GapGenerator> struct shell_sort {
  shell_sort(BDIterator first, BDIterator last) {
    using dist_type = typename std::iterator_traits<BDIterator>::difference_type;
    using T = typename std::iterator_traits<BDIterator>::value_type;
    dist_type size = std::distance(first, last);

    auto next_gap = GapGenerator{size};

    dist_type gap = next_gap();

    while (true) {
      for (dist_type start = 0; start < gap; start++) {
        for (dist_type first_unsorted = gap + start; first_unsorted < size;
             first_unsorted += gap) {

          BDIterator it = std::next(first, first_unsorted);

          T elem = std::move(*it);

          // searching for the insertion place and moving at the same time
          while (it != std::next(first, start) && elem < *std::prev(it, gap)) {
            *it = std::move(*std::prev(it, gap));
            std::advance(it, -gap);
          }

          *it = std::move(elem);
        }
      }
      if (gap == 1)
        break;
      gap = next_gap();
    }
  }
};

template <class DiffT> struct shell_gap { // original Shell's gaps of [N/2^k]
  shell_gap(DiffT n) : _n(n) {}
  DiffT operator()() { return _n /= 2; }

private:
  DiffT _n;
};

template <class BDIterator>
struct shell_sort_v1
    : shell_sort<BDIterator, shell_gap<typename BDIterator::difference_type>> {
  using Base = shell_sort<BDIterator, shell_gap<typename BDIterator::difference_type>>;

  shell_sort_v1(BDIterator first, BDIterator last) : Base{first, last} {}
};

template <class DiffT> struct papernov_gap { // 2^k + 1 gaps
  papernov_gap(DiffT n) {
    DiffT k = (std::log2(n - 1));
    _d = std::pow(2, k);
    _gap = _d + 1;
  }
  DiffT operator()() {
    if (auto gap = _gap; gap >= 3) {
      update();
      return gap;
    } else
      return 1;
  }

private:
  void update() { _gap -= (_d /= 2); }
  DiffT _d;
  DiffT _gap;
};

template <class BDIterator>
struct shell_sort_v2
    : shell_sort<BDIterator, papernov_gap<typename BDIterator::difference_type>> {
  using Base = shell_sort<BDIterator, papernov_gap<typename BDIterator::difference_type>>;

  shell_sort_v2(BDIterator first, BDIterator last) : Base{first, last} {}
};

template <class DiffT> struct sedgewick_gap { //  gaps of 4^k + 3*2^(k-1) + 1
  sedgewick_gap(DiffT n) {
    _gaps.push_back(1);
    _gaps.push_back(8);

    DiffT d_pow_of_2 = 2;
    while (_gaps.back() < n) {
      _gaps.push_back(_gaps.back() + 3 * (d_pow_of_2 * d_pow_of_2 + d_pow_of_2 / 2));
      d_pow_of_2 *= 2;
    }
    _gap_n = _gaps.size() - 1;
  }
  DiffT operator()() {
    if (_gap_n > 0)
      --_gap_n;
    return _gaps[_gap_n];

  }

private:
  std::vector<DiffT> _gaps;
  size_t _gap_n;
};

template <class BDIterator>
struct shell_sort_v3
    : shell_sort<BDIterator, sedgewick_gap<typename BDIterator::difference_type>> {
  using Base = shell_sort<BDIterator, sedgewick_gap<typename BDIterator::difference_type>>;

  shell_sort_v3(BDIterator first, BDIterator last) : Base{first, last} {}
};


#endif /* SORT_HPP */
