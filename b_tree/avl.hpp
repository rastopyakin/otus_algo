#ifndef AVL_HPP
#define AVL_HPP

#include "tree.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stack>
#include <utility>

template <class T>
struct AVLNode : public Node<T, AVLNode<T>> {
  AVLNode(const T &t, uint h = 1) : Node<T, AVLNode>{t}, height{h} {}
  uint height;
};

template <class T> class AVLTree : public Tree<AVLNode<T>> {

  using node_t = AVLNode<T>;
  using Tree<node_t>::root;

public:
  void insert(T t) { insert(t, root); }
  void remove(T t) { remove(t, root); }

  using Tree<node_t>::display;
  using Tree<node_t>::search;

  void check_heights() const { check_heights(root); }

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

  int height(const std::unique_ptr<node_t> &_node) const {
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

  void insert(T t, std::unique_ptr<node_t> &_root) {
    if (!_root)
      _root = std::make_unique<node_t>(t);
    if (t < _root->payload) {
      insert(t, _root->left);
    } else if (t > _root->payload) {
      insert(t, _root->right);
    }

    keep_balance(_root);
  }

  void remove(T t, std::unique_ptr<node_t> &_root) {
    if (!_root)
      return;

    if (t < _root->payload)
      remove(t, _root->left);
    else if (_root->payload < t)
      remove(t, _root->right);
    else {
      remove_node(_root);
    }

    if (_root)
      keep_balance(_root);
  }

  void remove_node(std::unique_ptr<node_t> &node) {
    if (!node->left || !node->right) {
      auto &child = node->left ? node->left : node->right;
      node = std::move(child);
      return;
    }

    std::unique_ptr<node_t> *min_in_right = &node->right;
    std::stack<std::unique_ptr<node_t>*> parents_to_balance;
    parents_to_balance.push(min_in_right);

    while ((*min_in_right)->left) {
      min_in_right = &((*min_in_right)->left);
      parents_to_balance.push(min_in_right);
    }

    // TODO: the following is essentially unwinding recursion, but is it possible to write
    // this down as a recursive function, which balances all the visited nodes and deletes
    // only the requested one?
    std::swap(node->payload, (*min_in_right)->payload);
    remove_node(*min_in_right);
    parents_to_balance.pop();

    while (!parents_to_balance.empty()) {
      keep_balance(*parents_to_balance.top());
      parents_to_balance.pop();
    }
  }

  void check_heights(const std::unique_ptr<node_t> &_root) const {
    if (!_root)
      return;
    check_heights(_root->left);
    int balance = height(_root->right) - height(_root->left);
    if (balance < -1 || balance > 1) {
      std::cerr << _root->payload << ": unbalanced node\n";
      std::cerr << "balance: " << balance << "\n";
    }
    check_heights(_root->right);
  }

};

#endif /* AVL_HPP */
