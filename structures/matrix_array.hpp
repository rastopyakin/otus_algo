#ifndef MATRIX_ARRAY_HPP
#define MATRIX_ARRAY_HPP

#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <utility>

template <class T, template <class> class Chunk> class MatrixArray {
public:
  MatrixArray() : _chunks{} { _chunks.add_back(IndexChunk{}); }
  ~MatrixArray() = default;

  using value_type = T;

  struct IndexChunk : public Chunk<T> {
    IndexChunk() = default;
    IndexChunk(IndexChunk &&) = default;
    IndexChunk &operator=(IndexChunk &&) = default;
    IndexChunk(Chunk<T> &&chunk, size_t cum_size)
        : Chunk<T>{std::move(chunk)}, _cum_size(cum_size) {}
    size_t _cum_size = 0;
  };

  void showSizes() {
    std::cerr << "cumsizes : ";
    for (int i = 0; i < _chunks.size(); i++)
      std::cerr << _chunks.get(i)._cum_size << " ";
    std::cerr << "\n";
  }

  void add_back(T item) {
    if (_chunks.back().size() >= chunk_size) {
      _chunks.add_back(IndexChunk{});
      upd_cum_sizes_from(n_chunks() - 2);
    }

    _chunks.back().add_back(item);

    _size++;
  }

  void add(T item, size_t pos) {
    size_t n_chunk = locate_chunk(pos);
    IndexChunk &chunk = _chunks.get(n_chunk);

    size_t cs = chunk._cum_size;
    chunk.add(item, pos - cs);
    _size++;

    if (chunk.size() >= 2 * chunk_size) {
      Chunk<T> insertee = chunk.crop_tail(chunk.size() / 2);
      size_t cs = chunk._cum_size + chunk.size();
      _chunks.add(IndexChunk{std::move(insertee), cs}, n_chunk + 1);
    }

    upd_cum_sizes_from(n_chunk);
  }

  T &get(size_t pos) {
    size_t n_chunk = locate_chunk(pos);
    return _chunks.get(n_chunk).get(pos - _chunks.get(n_chunk)._cum_size);
  }

  T remove(size_t pos);

  size_t size() const { return _size; }
  size_t n_chunks() const { return _chunks.size(); };

public:
  static constexpr size_t chunk_size = 2047;

private:
  size_t locate_chunk(size_t pos) {
    // the case of last chunk
    if (size_t last_cs = _chunks.back()._cum_size; pos >= last_cs)
      return n_chunks() - 1;

    auto is_between = [](size_t pos, size_t a, size_t b) {
      return a <= pos && pos < b;
    };

    size_t result = -1;
    std::function<void(int, int)> locate = [&](int i1, int i2) {
      if (is_between(pos, _chunks.get(i1)._cum_size,
                     _chunks.get(i2)._cum_size)) {
        if (i2 - i1 == 1) {
          result = i1;
          return;
        }
        int is = (i1 + i2) / 2;
        locate(i1, is);
        locate(is, i2);
      }
    };
    locate(0, n_chunks() - 1);
    return result;
  }

  void upd_cum_sizes_from(size_t pos) {
    size_t cs = _chunks.get(pos)._cum_size;
    for (size_t i = pos; i < _chunks.size(); i++) {
      _chunks.get(i)._cum_size = cs;
      cs += _chunks.get(i).size();
    }
  }

private:
  size_t _size = 0;
  Chunk<IndexChunk> _chunks;
};

#endif /* MATRIX_ARRAY_HPP */
