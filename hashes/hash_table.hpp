#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <list>
#include <forward_list>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

inline size_t constrain_hash(size_t hash, size_t n_bkt) { return hash % n_bkt; }

struct SimpleRehashPolicy {
  using size_type = std::size_t;
  SimpleRehashPolicy(float lf = 1.0) : _max_load_factor(lf) {}

  std::pair<bool, std::size_t> need_rehash(size_type n_bkt, size_type n_elem,
                                           size_type n_ins) const {
    if (n_elem + n_ins > std::floor(n_bkt * _max_load_factor)) {
      float n_bkt_min = (n_elem + n_ins) / _max_load_factor;
      return {true, std::max<std::size_t>(std::floor(n_bkt_min) + 1, n_bkt * 2)};
    }
    return {false, 0};
  }

  float max_load_factor() const { return _max_load_factor; }
  void max_load_factor(float new_factor) { _max_load_factor = new_factor; }

  float _max_load_factor;
};

struct PrimeRehashPolicy {};

template <class Key, class Value> struct HashTableNode {
  using value_type = std::pair<const Key, Value>;
  using key_type = Key;
  using mapped_type = Value;

  template <class K, class V>
  HashTableNode(K &&k, V &&v, std::size_t hash)
      : _kv_pair{std::forward<K>(k), std::forward<V>(v)}, _cached_hash(hash) {}
  HashTableNode(const HashTableNode &) = default;
  HashTableNode(HashTableNode &&) noexcept = default;

  mapped_type &value() { return _kv_pair.second; }
  const mapped_type &value() const { return _kv_pair.second; }
  const key_type &key() const { return _kv_pair.first; }

  value_type _kv_pair;
  std::size_t _cached_hash;
};

template <class Key, class Value, class RehashPolicy, class Hash, class Equal> class HashTable_impl;

template <class Key, class Value, class RehashPolicy, class Hash, class Equal>
class HashTableIterator_v0 {
  using _hash_table = HashTable_impl<Key, Value, RehashPolicy, Hash, Equal>;
  friend _hash_table;
  using _local_iterator = typename _hash_table::_local_iterator;
  using _size_type = typename _hash_table::size_type;

public:
  using value_type = typename _hash_table::value_type;
  using difference_type = typename _local_iterator::difference_type;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = typename _local_iterator::iterator_category;
  // bla-bla

public:
  HashTableIterator_v0(_hash_table *ht, _local_iterator curr_node, _size_type curr_bkt)
      : _ht(ht), _curr{curr_node}, _curr_bkt(curr_bkt) {}

  value_type &operator*() { return _curr->_kv_pair; }
  value_type *operator->() { return &(_curr->_kv_pair); }

  HashTableIterator_v0 operator++() {
    _curr++;

    if (_curr == _ht->_buckets[_curr_bkt].end()) {

      if (_curr_bkt < _ht->bucket_count() - 1) {
        _curr_bkt++;
        _curr = _ht->_buckets[_curr_bkt].begin();
      }

      while (_ht->_buckets[_curr_bkt].empty() && _curr_bkt < _ht->bucket_count()) {
        _curr_bkt++;
        if (_curr_bkt < _ht->bucket_count())
          _curr = _ht->_buckets[_curr_bkt].begin();
      }

      return *this;
    }

    return *this;
  }

  HashTableIterator_v0 operator++(int) {
    auto tmp{*this};
    this->operator++();
    return tmp;
  }

  bool operator==(const HashTableIterator_v0 &other) { return _curr == other._curr; }
  bool operator!=(const HashTableIterator_v0 &other) { return _curr != other._curr; }

private:
  _hash_table *_ht;
  _local_iterator _curr;
  _size_type _curr_bkt;
};

template <class Key, class Value, class RehashPolicy, class Hash, class Equal>
class HashTable_impl {
public:
  using node_type = HashTableNode<Key, Value>;
  using value_type = typename node_type::value_type;
  using bucket_type = std::list<node_type>;
  using _local_iterator = typename bucket_type::iterator;

  using key_type = typename node_type::key_type;
  using mapped_type = typename node_type::mapped_type;

  using iterator = HashTableIterator_v0<Key, Value, RehashPolicy, Hash, Equal>;
  friend iterator;
  using size_type = typename bucket_type::size_type;

public:
  HashTable_impl() : _buckets{}, _count(0), _rehash_pol(1.0), _hash(), _eq() {
    _buckets.emplace_back(); // let's have at least one bucket in the beginning to avoid checking
                             // division by zero upon hash constraining and for keeping consistency
  }

  void rehash(size_type n_bkt) {
    std::vector<bucket_type> old_buckets(n_bkt);
    old_buckets.swap(_buckets);

    for (auto bucket = old_buckets.begin(); bucket != old_buckets.end(); bucket++)
      for (auto node = bucket->begin(); node != bucket->end(); node++)
        insert(std::move(*node));
  }

  size_type size() const { return _count; }
  size_type bucket_count() const { return _buckets.size(); }
  const bucket_type &bucket(size_type bkt_n) { return _buckets[bkt_n]; }
  size_type bucket_size(size_type n) const { return _buckets[n].size(); }
  float max_load_factor() const { return _rehash_pol.max_load_factor(); }
  float load_factor() const {
    return static_cast<float>(size()) / static_cast<float>(bucket_count());
  }

  template <class K, class V> std::pair<iterator, bool> insert_or_assign(K &&k, V &&v) {
    if (iterator i = find(k); i != end()) {
      i->second = v;
      return {i, false};
    }

    if (auto [need_rehash, n_bkt_new] = _rehash_pol.need_rehash(bucket_count(), size(), 1);
        need_rehash) {
      rehash(n_bkt_new);
    }

    std::size_t hash = _hash(k);
    size_type bkt = constrain_hash(hash, bucket_count());
    bucket_type &bucket = _buckets[bkt];

    bucket.emplace_front(std::forward<K>(k), std::forward<V>(v), hash);
    _count++;

    return {iterator{this, bucket.begin(), bkt}, true};
  }

  mapped_type &operator[](const key_type &key) {
    if (iterator i = find(key); i != end()) {
      return i->second;
    }

    if (auto [need_rehash, n_bkt_new] = _rehash_pol.need_rehash(bucket_count(), size(), 1);
        need_rehash) {
      rehash(n_bkt_new);
    }

    std::size_t hash = _hash(key);
    size_type bkt = constrain_hash(hash, bucket_count());
    bucket_type &bucket = _buckets[bkt];

    bucket.emplace_front(key, mapped_type{}, hash);
    _count++;
    return bucket.front().value();
  }

  const mapped_type &operator[](const key_type &key) const; // not implemented

  bool erase(const Key &k) {
    iterator pos = find(k);
    if (pos == end())
      return false;

    erase(pos);
    return true;
  }

  void erase(iterator pos) {
    _buckets[pos._curr_bkt].erase(pos._curr);
    --_count;
  }

  iterator find(const key_type &k) {
    std::size_t hash = _hash(k);
    size_type bkt = constrain_hash(hash, bucket_count());
    bucket_type &bucket = _buckets[bkt];

    for (_local_iterator i = bucket.begin(); i != bucket.end(); i++)
      if (hash == i->_cached_hash)
        if (_eq(i->key(), k))
          return iterator{this, i, bkt};

    return end();
  }

  iterator end() { return iterator{this, _buckets.back().end(), bucket_count() - 1}; }
  iterator begin() {
    size_type bkt = 0;
    while (_buckets[bkt].empty() && bkt < bucket_count() - 1) {
      bkt++;
    }
    return iterator{this, _buckets[bkt].begin(), bkt};
  }

private:
  void insert(node_type &&node) {
    bucket_type &bucket = _buckets[constrain_hash(node._cached_hash, bucket_count())];

    bucket.emplace_front(std::move(node));
  }

  using rehash_policy = RehashPolicy;
  using hasher = Hash;
  using equal_pred = Equal;

private:
  std::vector<bucket_type> _buckets;
  size_type _count;

  rehash_policy _rehash_pol;
  hasher _hash;
  equal_pred _eq;
};

template <class Key, class Value>
class HashTable
    : public HashTable_impl<Key, Value, SimpleRehashPolicy, std::hash<Key>, std::equal_to<Key>> {};

#endif /* HASH_TABLE_HPP */
