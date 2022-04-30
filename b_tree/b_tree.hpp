#ifndef B_TREE_HPP
#define B_TREE_HPP

#include <utility>

#include "tree.hpp"

template <class T>
struct BSTNode : public Node<T, BSTNode<T>> {
  BSTNode(const T &t) : Node<T, BSTNode>{t} {}
};

template <class T> class BSTree : public Tree<BSTNode<T>>{
protected:
  using node_t = BSTNode<T>;
  using Tree<node_t>::root;

public:

  void insert(T t) {
    if (auto &node = search(t, root); !node)
      node = std::make_unique<node_t>(t);
  }

  void remove(const T &e) {
    if (auto &node = search(e, root); node)
      remove_node(node);
  }

  using Tree<node_t>::display;
  using Tree<node_t>::search;

private:

  void remove_node(std::unique_ptr<node_t> &node) {

    if (!node->left || !node->right) {
      auto &child = node->left ? node->left : node->right;
      node = std::move(child);
      return;
    }

    std::unique_ptr<node_t> *min_in_right = &node->right;

    while ((*min_in_right)->left) {
      min_in_right = &((*min_in_right)->left);
    }

    std::swap(node->payload, (*min_in_right)->payload);
    remove_node(*min_in_right);
  }
};

#endif /* B_TREE_HPP */
