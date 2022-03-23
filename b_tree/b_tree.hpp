#ifndef B_TREE_HPP
#define B_TREE_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <queue>
#include <string>
#include <utility>

template <class T> class BSTree {
  struct node {
    node(T t) : payload{t} {}
    T payload;
    node *parent = nullptr;
    std::unique_ptr<node> left, right;
  };

public:
  void insert(T t) { root = insert(t, root); }

  void traverse() { traverse(root.get()); }

  void display() const;

private:
  // TODO: check if there is performance penalty due to pointers passing
  std::unique_ptr<node> insert(T t, std::unique_ptr<node> &_root) {
    if (!_root)
      return std::make_unique<node>(t);
    if (t < _root->payload) {
      _root->left = insert(t, _root->left);
      _root->left->parent = _root.get();
    } else {
      _root->right = insert(t, _root->right);
      _root->right->parent = _root.get();
    }
    return std::move(_root);
  }

  void traverse(node *_root) {
    if (!_root)
      return;
    traverse(_root->left.get());
    std::cout << _root->payload << ", parent: ";
    if (_root->parent)
      std::cout << _root->parent->payload;
    else
      std::cout << "null";
    std::cout << "\n";
    traverse(_root->right.get());
  }

private:
  std::unique_ptr<node> root;
  void foo();
};

template <class T> struct node_pos_t {
  int level;
  int position;
  T payload;
  node_pos_t *parent = nullptr;
  node_pos_t *l = nullptr;
  node_pos_t *r = nullptr;
};

template <class T> void shift_positions(int shift, node_pos_t<T> *root) {
  root->position += shift;
  if (root->l)
    shift_positions(shift, root->l);
  if (root->r)
    shift_positions(shift, root->r);
}

template <class Node> int node_width(const Node &t) {
  return std::to_string(t.payload).length();
}

template <class T>
bool check_positions(node_pos_t<T> *p_l, node_pos_t<T> *p_r) {

  bool need_fix = p_r->position <= p_l->position + node_width(*p_l);
  if (need_fix) {

    int shift = p_l->position + node_width(*p_l) - p_r->position;
    // std::cout << "shift: " << shift << ", node: " << p_r->payload;
    // std::cout << " ,l: " << p_r->l->payload << "\n";

    node_pos_t<T> *root_to_shift = p_r;
    while (root_to_shift->parent->l == root_to_shift) {
      root_to_shift = root_to_shift->parent;
    }

    shift_positions(shift / 2 + 1, root_to_shift);

    root_to_shift = p_l;
    while (root_to_shift->parent->r == root_to_shift) {
      root_to_shift = root_to_shift->parent;
    }

    shift_positions(-shift / 2 - 1, root_to_shift);
  }

  return !need_fix;
}

template <template <class> class Cont, class T>
bool check_level(int level, Cont<node_pos_t<T>> &positions) {
  auto is_lvl = [level](auto &pos) { return pos.level == level; };

  auto l_beg = std::find_if(positions.begin(), positions.end(), is_lvl);

  auto l_end = --std::find_if_not(l_beg, positions.end(), is_lvl);

  bool stable = true;

  while (l_beg != l_end) {
    if (!check_positions(&(*l_beg), &(*std::next(l_beg))))
      stable = false;
    l_beg++;
  }

  return stable;
}

template <class T> void BSTree<T>::display() const {

  using node_pos = node_pos_t<T>;

  std::list<node_pos> positions;

  std::queue<BSTree<T>::node *> nodes;
  nodes.push(root.get());

  std::queue<node_pos> pos_queue;
  pos_queue.push(node_pos{.level = 0, .position = 0, .payload = root->payload});

  while (!nodes.empty()) {
    BSTree<T>::node *node = nodes.front();
    nodes.pop();

    node_pos pos = pos_queue.front();

    node_pos **child_upd = nullptr;
    if (pos.parent)
      child_upd = (pos.parent->r == &pos_queue.front()) ? &pos.parent->r
                                                        : &pos.parent->l;
    pos_queue.pop();

    positions.push_back(pos);
    if (child_upd)
      *child_upd = &positions.back();

    if (node->left) {
      nodes.push(node->left.get());
      pos_queue.push(
          node_pos{.level = pos.level + 1,
                   .position = pos.position - 1 - node_width(*node->left),
                   .payload = node->left->payload,
                   .parent = &positions.back()});
      positions.back().l = &pos_queue.back();
    }

    if (node->right) {
      nodes.push(node->right.get());
      pos_queue.push(node_pos{.level = pos.level + 1,
                              .position = pos.position + 1 + node_width(*node),
                              .payload = node->right->payload,
                              .parent = &positions.back()});
      positions.back().r = &pos_queue.back();
    }

    for (int level = pos.level; level > 0; level--)
      check_level(level, positions);
    check_overlaps(positions);
  }

  // bool stable = false;
  // while (!stable) {
  // for (int level = positions.back().level; level > 0; level--)
  // stable = check_level(level, positions);
  // stable = check_overlaps(positions);
  // }

  int min_pos =
      std::min_element(positions.begin(), positions.end(),
                       [](auto &a, auto &b) { return a.position < b.position; })
          ->position;

  for (auto &pos : positions) {
    pos.position += -min_pos;
  }

  draw(positions);
}

template <template <class> class Cont, class T>
bool check_overlaps(Cont<node_pos_t<T>> &positions) {

  bool stable = true;
  for (auto i = std::next(positions.begin()); i != std::prev(positions.end());
       i++) {
    if (auto prev = std::prev(i); i->l && (prev->level == i->level))
      if (i->l->position <= prev->position + node_width(*prev)) {
        // std::cout << prev->payload << " ";
        int shift = prev->position + node_width(*prev) - i->l->position;
        node_pos_t<T> *root_to_shift = &(*i);
        while (root_to_shift->parent->l == root_to_shift) {
          root_to_shift = root_to_shift->parent;
        }

        shift_positions(shift / 2 + 1, root_to_shift);

        root_to_shift = &(*prev);
        while (root_to_shift->parent->r == root_to_shift) {
          root_to_shift = root_to_shift->parent;
        }

        shift_positions(-shift / 2 - 1, root_to_shift);

        stable = false;
      }

    if (auto next = std::next(i); i->r && (next->level == i->level))
      if (i->r->position + node_width(*i->r) >= next->position) {
        // std::cout << next->payload << " ";
        int shift = i->r->position + node_width(*i->r) - next->position;
        node_pos_t<T> *root_to_shift = &(*next);
        while (root_to_shift->parent->l == root_to_shift) {
          root_to_shift = root_to_shift->parent;
        }

        shift_positions(shift / 2 + 1, root_to_shift);

        root_to_shift = &(*i);
        while (root_to_shift->parent->r == root_to_shift) {
          root_to_shift = root_to_shift->parent;
        }

        shift_positions(-shift / 2 - 1, root_to_shift);
        stable = false;
      }
  }

  return stable;
  std::cout << "\n";
}

template <template <class> class Cont, class T>
void draw(const Cont<node_pos_t<T>> &positions) {
  int level = positions.front().level;

  std::string nodes, edges;

  for (const auto &pos : positions) {
    if (level != pos.level) {
      std::cout << nodes << "\n" << edges << "\n";
      nodes.clear();
      edges.clear();
      level = pos.level;
    }

    int spaces = pos.position - nodes.length();
    int lines = 0;
    std::string pld_str = std::to_string(pos.payload);

    if (pos.l) {
      int ed_pos = pos.l->position + node_width(*pos.l) / 2 + 1;
      lines = pos.position - ed_pos - 1;
      spaces -= lines;
      edges += std::string(ed_pos - edges.length(), ' ') + '/';
    }

    nodes += std::string(spaces, ' ');
    nodes += std::string(lines, '_');
    nodes += pld_str;

    if (pos.r) {
      int ed_pos = pos.r->position + node_width(*pos.r) / 2 - 1;
      lines = ed_pos - nodes.length();
      nodes += std::string(lines, '_');
      edges += std::string(ed_pos - edges.length(), ' ') + '\\';
    }
  }
  std::cout << nodes;
  std::cout << "\n";
}

#endif /* B_TREE_HPP */
