#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>

template <class T, class ConcreteNode,
          class NodePtr = std::unique_ptr<ConcreteNode>>
struct Node {
  using payload_type = T;
  using pointer_type = NodePtr;

  Node(const T &t) : payload{t} {}
  T payload;
  NodePtr left{nullptr}, right{nullptr};

  std::string display_str() const { return display_str(payload); };

private:
  template <class U> std::string display_str(const U &_payload) const {
    return std::to_string(payload);
  }
  std::string display_str(char c) const { return {c}; }
  const std::string &display_str(const std::string &str) const { return str; }
};

#endif /* NODE_HPP */
