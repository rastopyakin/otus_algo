#ifndef AVL_HPP
#define AVL_HPP

#include "b_tree.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <utility>

template <class T> class AVLTree : public BSTree<T> {

  struct node_t : public Node<T, node_t> {
    node_t(const T &t, uint h = 1) : Node<T, node_t>{t}, height{h} {}

    uint height;
  };

public:
  void insert(T t) { root = insert(t, root); }
  void display() const { BSTree<T>::display(root.get()); }

  void heights() const { heights(root.get()); }

private:

  void smallLeftRotation(std::unique_ptr<node_t> &_root) {
    auto pivot = std::move(_root->right);

    _root->right = std::move(pivot->left);
    _root->height = std::max(height(_root->left), height(_root->right)) + 1;

    auto tmp = std::move(_root);
    _root = std::move(pivot);

    _root->left = std::move(tmp);
    _root->height = std::max(height(_root->left), height(_root->right)) + 1;
  }

  void smallRightRotation(std::unique_ptr<node_t> &_root) {
     auto pivot = std::move(_root->left);

    _root->left = std::move(pivot->right);
    _root->height = std::max(height(_root->left), height(_root->right)) + 1;

    auto tmp = std::move(_root);
    _root = std::move(pivot);

    _root->right = std::move(tmp);
    _root->height = std::max(height(_root->left), height(_root->right)) + 1;
  }

  void bigLeftRotation(std::unique_ptr<node_t> &_root) {
    smallRightRotation(_root->right);
    smallLeftRotation(_root);
  }
  void bigRightRotation(std::unique_ptr<node_t> &_root) {
    smallLeftRotation(_root->left);
    smallRightRotation(_root);
  }

  int height(std::unique_ptr<node_t> &_node) {
    return _node ? _node->height : 0;
  }

  void keep_balance(std::unique_ptr<node_t> &_root) {

    if (height(_root->right) > height(_root->left) + 1) {
      if (height(_root->right->right) >= height(_root->right->left))
        smallLeftRotation(_root);
      else
        bigLeftRotation(_root);
    }

    if (height(_root->left) > height(_root->right) + 1) {
      if (height(_root->left->left) >= height(_root->left->right))
        smallRightRotation(_root);
      else
        bigRightRotation(_root);
    }

    _root->height = std::max(height(_root->left), height(_root->right)) + 1;
  }

  std::unique_ptr<node_t> insert(T t, std::unique_ptr<node_t> &_root) {
    if (!_root)
      return std::make_unique<node_t>(t);
    if (t < _root->payload) {
      _root->left = insert(t, _root->left);
    } else if (t > _root->payload) {
      _root->right = insert(t, _root->right);
    }

    keep_balance(_root);

    return std::move(_root);
  }

  void heights(node_t *_root) const {
    if (!_root) return;
    heights(_root->left.get());
    std::cout << _root->payload << ": h" << _root->height << "\n";
    heights(_root->right.get());
  }

private:
  std::unique_ptr<node_t> root;
};

#endif /* AVL_HPP */
