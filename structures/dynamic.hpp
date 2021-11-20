#ifndef DYNAMIC_HPP
#define DYNAMIC_HPP

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>

template <class T, class Derived> class ReallocStrategy {
public:
  ReallocStrategy() = default;
  ReallocStrategy(size_t capacity) : _capacity(capacity) {}

  using pointer = std::unique_ptr<T[]>;

  pointer alloc() { return std::make_unique<T[]>(_capacity); }
  pointer realloc() {
    capacity_update();
    return alloc();
  }

  size_t capacity() const { return _capacity; }

protected:
  void capacity_update() { static_cast<Derived *>(this)->capacity_update(); }
  size_t _capacity = 0;
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
  void capacity_update() { this->_capacity = 1.5 * this->_capacity + 1; }
  friend class ReallocStrategy<T, FactorAlloc<T>>;
};

template <class T, class ReallocStrategy> class Array {
public:
  Array() = default;
  Array(size_t capacity) : _realloc{capacity}, _array{_realloc.alloc()} {}
  ~Array() = default;

  using pointer = typename ReallocStrategy::pointer;

  void add_back(T item) {
    resize();
    _array[_size] = item;
    _size++;
  }

  void add(T item, size_t index) {
    if (_size >= _realloc.capacity()) {
      auto new_array = _realloc.realloc();
      // std::cout << "Resized : " << _realloc.capacity() << "\n";
      std::copy_n(_array.get(), index, new_array.get());
      new_array[index] = item;
      std::copy_n(_array.get() + index, _size - index,
                  new_array.get() + index + 1);
      _array.swap(new_array);
    } else {
      std::move(_array.get() + index, _array.get() + _size,
                _array.get() + index + 1);
      _array[index] = item;
    }
    _size++;
  }

  T &get(size_t index) { return _array[index]; }

  void remove(size_t index) {}

  void resize() {
    if (_size >= _realloc.capacity()) {
      auto new_array = _realloc.realloc();
      std::copy_n(_array.get(), _size, new_array.get());
      _array.swap(new_array);
      // std::cout << "Resized: " << _realloc.capacity() << "\n";
    }
  }

  size_t size() const { return _size; }

private:
  size_t _size = 0;
  ReallocStrategy _realloc;
  pointer _array;
};

#endif /* DYNAMIC_HPP */
