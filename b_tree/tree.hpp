#ifndef TREE_HPP
#define TREE_HPP

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <string>

template <class T, class ConcreteNode> struct Node {
  using payload_t = T;

  Node(const T &t) : payload{t} {}
  T payload;
  std::unique_ptr<ConcreteNode> left, right;

  std::string display_str() const { return display_str(payload); };

private:
  template <class U> std::string display_str(const U &_payload) const {
    return std::to_string(payload);
  }
  std::string display_str(char c) const { return {c}; }
};

template <class Node> class Tree {
  using T = typename Node::payload_t;

public:
  bool search(const T &e) { return search(e, root) != nullptr; }
  void display() const { display(root.get()); }

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

  void display(const Node *_root) const;

protected:
  std::unique_ptr<Node> root;
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

  std::string display_str() const { return display_str(payload); };

private:
  template <class U> std::string display_str(const U &_payload) const {
    return std::to_string(payload);
  }
  std::string display_str(char c) const { return {c}; }
};

template <class T> void shift_positions(int shift, node_pos_t<T> *root) {
  root->position += shift;
  if (root->l)
    shift_positions(shift, root->l);
  if (root->r)
    shift_positions(shift, root->r);
}

template <class Node> int node_width(const Node &t) {
  return t.display_str().length();
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

template <class Node> void Tree<Node>::display(const Node *_root) const {

  using node_pos = node_pos_t<T>;

  if (!_root)
    return;

  std::list<node_pos> positions;

  std::queue<const Node *> nodes;

  nodes.push(_root);

  std::queue<node_pos> pos_queue;
  pos_queue.push(
      node_pos{.level = 0, .position = 0, .payload = _root->payload});

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
    std::string pld_str = pos.display_str();

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

#endif /* TREE_HPP */
