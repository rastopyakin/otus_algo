#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

template <class T, class Derived> class ReallocStrategy {
public:
  ReallocStrategy() = default;
  ReallocStrategy(size_t capacity)
      : _capacity(capacity), _array(std::make_unique<T[]>(_capacity)) {}
  ReallocStrategy(ReallocStrategy &&) = default;
  ReallocStrategy &operator=(ReallocStrategy &&) = default;

  using pointer = T *;

  pointer get() const { return _array.get(); };

  pointer extend_and_move(size_t n_copy) {
    capacity_update();
    auto new_array = std::make_unique<T[]>(capacity());
    std::move(_array.get(), _array.get() + n_copy, new_array.get());
    _array.swap(new_array);
    return get();
  }

  size_t capacity() const { return _capacity; }

  void swap(ReallocStrategy &other) {
    _array.swap(other._array);
    std::swap(_capacity, other._capacity);
  }

protected:
  void capacity_update() { static_cast<Derived *>(this)->capacity_update(); }
  size_t _capacity = 0;

private:
  std::unique_ptr<T[]> _array;
};

template <class T>
class SingleAlloc : public ReallocStrategy<T, SingleAlloc<T>> {
  void capacity_update() { this->_capacity++; }
  friend class ReallocStrategy<T, SingleAlloc<T>>;
};

template <class T>
class VectorAlloc : public ReallocStrategy<T, VectorAlloc<T>> {
  void capacity_update() { this->_capacity += 100; }
  friend class ReallocStrategy<T, VectorAlloc<T>>;
};

template <class T>
class FactorAlloc : public ReallocStrategy<T, FactorAlloc<T>> {
  void capacity_update() { this->_capacity = 2 * this->_capacity + 1; }
  friend class ReallocStrategy<T, FactorAlloc<T>>;
};

template <class T, class ReallocStrategy> class Array {
public:
  Array() = default;
  Array(size_t capacity) : _realloc{capacity}, _array(_realloc.get()) {}
  ~Array() = default;

  void swap(Array &other) noexcept {
    _realloc.swap(other._realloc);
    std::swap(_size, other._size);
    std::swap(_array, other._array);
  }

  Array(Array &&other) noexcept : Array{} { this->swap(other); }

  Array &operator=(Array &&other) noexcept {
    swap(other);
    Array<T, ReallocStrategy>{}.swap(other);
    return *this;
  }

  using pointer = typename ReallocStrategy::pointer;
  using value_type = T;

  template <class U> void add_back(U &&item) {
    maybe_realloc();
    _array[_size] = std::forward<U>(item);
    _size++;
    // add(item, _size);
  }

  template <class U> void add(U &&item, size_t index) {
    maybe_realloc();
    std::move_backward(_array + index, _array + _size, _array + _size + 1);
    _array[index] = std::forward<U>(item);
    _size++;
  }

  Array crop_tail(size_t from_pos) {
    Array retval{_size - from_pos};
    retval._size = _size - from_pos;
    std::move(_array + from_pos, _array + _size, retval._array);
    _size = from_pos;
    return retval;
  }

  T &back() { return _array[_size - 1]; }

  T &get(size_t index) { return _array[index]; }

  void remove(size_t index) {}

  size_t size() const { return _size; }

private:
  void maybe_realloc() {
    if (_size >= _realloc.capacity()) {
      _array = _realloc.extend_and_move(_size);
    }
  }

private:
  size_t _size = 0;
  ReallocStrategy _realloc;
  pointer _array;
};

#endif /* DYNAMIC_ARRAY_HPP */
