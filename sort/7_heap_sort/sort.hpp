#ifndef SORT_HPP
#define SORT_HPP

#include <iterator>
#include <type_traits>

template <class BDIterator> void selection_sort(BDIterator first, BDIterator last) {

  auto find_max = [&first](BDIterator last_unsorted) {
    BDIterator max = first;
    for (BDIterator it = first; it != last_unsorted; std::advance(it, 1))
      if (*it > *max)
        max = it;

    return max;
  };

  while (last != first) {
    BDIterator max = find_max(last);
    std::advance(last, -1);
    std::swap(*max, *last);
  }
}

template <class BDIterator,
          class DiffT = typename std::iterator_traits<BDIterator>::difference_type>
void restore_heap(BDIterator root, DiffT index, DiffT size) {

  auto max_it = [](BDIterator a, BDIterator b) { return *a > *b ? a : b; };

  while (2*index + 1 < size) {
    BDIterator left = std::next(root, index + 1);
    BDIterator right = std::next(left);

    BDIterator max = max_it(root, left);

    if (2*index + 2 < size)
      max = max_it(max, right);

    if (max == root)
      break;
    std::swap(*root, *max);

    if (max == left) {
      index = 2*index + 1;
      root = left;
    }

    if (max == right) {
      index = 2*index + 2;
      root = right;
    }

  }
}

template <class BDIterator> void heapify(BDIterator first, BDIterator last) {

  auto max_it = [](BDIterator a, BDIterator b) { return *a > *b ? a : b; };

  using difference_type = typename std::iterator_traits<BDIterator>::difference_type;
  difference_type size = std::distance(first, last);

  for (difference_type index = (size - 2) / 2; index >= 0; index--) {

    BDIterator root = std::next(first, index);

    BDIterator right = std::next(first, 2 * index + 2);
    BDIterator left = std::prev(right);
    BDIterator max = root;

    if (right != last)
      max = max_it(root, right);

    max = max_it(max, left);
    std::swap(*root, *max);

    if (max == left)
      restore_heap(left, 2*index + 1, size);

    if (max == right)
      restore_heap(right, 2*index + 2, size);

  }
}

template <class BDIterator> void heap_sort(BDIterator first, BDIterator last) {

  heapify(first, last);

  using difference_type = typename std::iterator_traits<BDIterator>::difference_type;
  difference_type size = std::distance(first, last);

  while (first != last) {
    std::advance(last, -1);
    std::swap(*first, *last);
    restore_heap(first, difference_type{0}, --size);
  }
}

#endif /* SORT_HPP */
