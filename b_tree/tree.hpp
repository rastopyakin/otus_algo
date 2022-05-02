#ifndef TREE_HPP
#define TREE_HPP

#include <memory>

#include "node.hpp"
#include "display.hpp"

template <class Node> class Tree {
  using T = typename Node::payload_type;

public:
  bool search(const T &e) { return search(e, root) != nullptr; }
  void display() const { display_tree(root.get()); }

protected:
  std::unique_ptr<Node> &search(const T &e, std::unique_ptr<Node> &_root) {
    if (!_root)
      return _root;

    if (e < _root->payload)
      return search(e, _root->left);
    else if (e > _root->payload)
      return search(e, _root->right);
    else
      return _root;
  }

protected:
  std::unique_ptr<Node> root;
};

#endif /* TREE_HPP */
