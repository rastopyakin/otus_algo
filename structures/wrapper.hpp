#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <iterator>

template <class Container> class Wrapper {
public:
  using value_type = typename Container::value_type;

  Wrapper() = default;
  ~Wrapper() = default;

  void add_back(value_type item) { _cont.push_back(item); }

  void add(value_type &item, size_t index) {
    auto it = std::cbegin(_cont);
    std::advance(it, index);
    _cont.insert(it, item);
  }

  value_type &get(size_t index) {
    auto it = std::begin(_cont);
    std::advance(it, index);
    return *it;
  }

  void remove() {}

  size_t size() const { return _cont.size(); }

private:
  Container _cont;
};


#endif /* WRAPPER_HPP */
