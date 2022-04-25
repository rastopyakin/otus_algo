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
#include <vector>

template <class T, class ConcreteNode> struct Node {
  Node(const T &t) : payload{t} {}
  T payload;
  std::unique_ptr<ConcreteNode> left, right;
};

template <class T> class BSTree {
protected:

  struct node_t : public Node<T, node_t> {
    node_t(const T &t) : Node<T, node_t>{t} {}
  };

public:
  void insert(T t) { root = insert(t, std::move(root)); }

  void traverse() { traverse(root.get()); }

  void display() const { display(root.get()); }

  bool search(const T &e) { return search(e, root.get()); }

  void remove(const T &e) { remove(e, root); }

protected:
  // TODO: check if there is performance penalty due to pointers passing
  template <class Node>
  std::unique_ptr<Node> insert(T t, std::unique_ptr<Node> &&_root) {
    if (!_root)
      return std::make_unique<Node>(t);
    if (t < _root->payload) {
      _root->left = insert(t, std::move(_root->left));
    } else if (t > _root->payload) {
      _root->right = insert(t, std::move(_root->right));
    }
    return std::move(_root);
  }

  bool search(const T &e, node_t *_root) {
    if (!_root)
      return false;

    if (e < _root->payload)
      return search(e, _root->left.get());
    else if (e > _root->payload)
      return search(e, _root->right.get());
    else
      return true;
  }

  void remove(const T &e, std::unique_ptr<node_t> &_root,
              node_t *parent = nullptr) {
    if (!_root)
      return;

    if (e < _root->payload)
      remove(e, _root->left, _root.get());
    else if (e > _root->payload)
      remove(e, _root->right, _root.get());
    else
      remove_node(_root.get(), parent);
  }

  void remove_node(node_t *node, node_t *parent) {
    if (node->left && node->right) {
      node_t *min_in_right = node->right.get();
      parent = node;
      while (min_in_right->left) {
        parent = min_in_right;
        min_in_right = min_in_right->left.get();
      }

      std::swap(node->payload, min_in_right->payload);
      node = min_in_right;
    }

    auto &child = node->left ? node->left : node->right;

    if (!parent)
      root = std::move(child);
    else if (parent->left.get() == node)
      parent->left = std::move(child);
    else
      parent->right = std::move(child);
  }

  template<class Node> void display(const Node * _root) const;

protected:
  std::unique_ptr<node_t> root;
};

// TODO: the tree displaying code below is a mess, express better using
// operations the tree has

// node_pos_t is the same tree node with additional info: level, position,
// parent shift_positions: shifts positions of all children of a given root
// check_positions: if to nodes overlap then shift them accordingly with the
// containing subtree
// check_level: checks all neigbour pair on a given level

// display: basically builds the tree with all additional info as it is in
// node_pos_t, then checks neigbours on all levels and overlaps between a node
// and its neigbour's child, which would make the tree edges impossible to draw

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

  // TODO: potential bug here: l_end will point to smth even if nothing found
  auto l_end = --std::find_if_not(l_beg, positions.end(), is_lvl);

  bool stable = true;

  while (l_beg != l_end) {
    if (!check_positions(&(*l_beg), &(*std::next(l_beg))))
      stable = false;
    l_beg++;
  }

  return stable;
}

template <template <class> class Cont, class T>
bool check_overlaps(Cont<node_pos_t<T>> &positions) {

  bool stable = true;
  if (std::distance(std::begin(positions), std::end(positions)) < 4)
    return true;

  for (auto i = positions.begin(); i != std::prev(positions.end()); i++) {
    if (auto next = std::next(i); next->l && (next->level == i->level))
      stable = check_positions(&(*i), &(*next->l));

    if (auto next = std::next(i); i->r && (next->level == i->level))
      check_positions(&(*i->r), &(*next));
  }

  return stable;
}

template <class T>
template <class Node> void BSTree<T>::display(const Node * _root) const {

  using node_pos = node_pos_t<T>;

  if (!_root)
    return;

  std::list<node_pos> positions;

  std::queue<const Node *> nodes;

  nodes.push(_root);

  std::queue<node_pos> pos_queue;
  pos_queue.push(node_pos{.level = 0, .position = 0, .payload = _root->payload});

  while (!nodes.empty()) {
    const Node *node = nodes.front();
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

    for (int level = pos.level; level > 0; level--) {
      check_level(level, positions);
    }
    check_overlaps(positions);
  }

  for (int level = positions.back().level; level > 0; level--) {
    check_level(level, positions);
    check_overlaps(positions);
  }

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
