#ifndef TREE_HPP
#define TREE_HPP

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <string>

template <class T, class ConcreteNode,
          class NodePtr = std::unique_ptr<ConcreteNode>>
struct Node {
  using payload_type = T;
  using pointer_type = NodePtr;

  Node(const T &t) : payload{t} {}
  T payload;
  NodePtr left {nullptr}, right {nullptr};

  std::string display_str() const { return display_str(payload); };

private:
  template <class U> std::string display_str(const U &_payload) const {
    return std::to_string(payload);
  }
  std::string display_str(char c) const { return {c}; }
  const std::string & display_str(const std::string &str) const { return str; }
};

template <class Node> class Tree {
  using T = typename Node::payload_type;

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

// DisplayNode is the same tree node with additional info: level, position,
// parent, and not owning pointers

// shift_positions(): shifts positions of all children of a given root

// check_positions(): if to nodes overlap then shift them accordingly with the
// containing subtree

// check_level(): checks all neigbour pair on a given level

// display(): basically builds the tree with all additional info as it is in DisplayNode,
// then checks neigbours on all tree's levels and overlaps between a node and its
// neigbour's child, which would make the tree arcs impossible to draw

// display() stores the nodes in a list, because (i) linear container makes the check
// easier to implement and (ii) list never reallocates

template <class T>
struct DisplayNode : public Node<T, DisplayNode<T>, DisplayNode<T> *> {
  using Base = Node<T, DisplayNode<T>, DisplayNode<T> *>;
  DisplayNode(const T &pld) : Base{pld} {}
  int level = 0;
  int position = 0;
  DisplayNode *parent = nullptr;
};

template <class T> void shift_positions(int shift, DisplayNode<T> *root) {
  root->position += shift;
  if (root->left)
    shift_positions(shift, root->left);
  if (root->right)
    shift_positions(shift, root->right);
}

template <class Node> int node_width(const Node &t) {
  return t.display_str().length();
}

template <class T>
void check_positions(DisplayNode<T> *p_l, DisplayNode<T> *p_r) {

  bool need_fix = p_r->position <= p_l->position + node_width(*p_l);
  if (need_fix) {

    int shift = p_l->position + node_width(*p_l) - p_r->position;

    DisplayNode<T> *root_to_shift = p_r;
    while (root_to_shift->parent->left == root_to_shift) {
      root_to_shift = root_to_shift->parent;
    }

    shift_positions(shift / 2 + 1, root_to_shift);

    root_to_shift = p_l;
    while (root_to_shift->parent->right == root_to_shift) {
      root_to_shift = root_to_shift->parent;
    }

    shift_positions(-shift / 2 - 1, root_to_shift);
  }
}

template <template <class> class Cont, class T>
void check_level(int level, Cont<DisplayNode<T>> &positions) {
  auto is_lvl = [level](auto &pos) { return pos.level == level; };

  auto l_beg = std::find_if(positions.begin(), positions.end(), is_lvl);

  // TODO: potential bug here: l_end will point to smth even if nothing found
  auto l_end = --std::find_if_not(l_beg, positions.end(), is_lvl);

  while (l_beg != l_end) {
    check_positions(&(*l_beg), &(*std::next(l_beg)));
    l_beg++;
  }
}

template <template <class> class Cont, class T>
void check_overlaps(Cont<DisplayNode<T>> &positions) {

  for (auto i = positions.begin(); i != std::prev(positions.end()); i++) {
    if (auto next = std::next(i); next->left && (next->level == i->level))
      check_positions(&(*i), &(*next->left));

    if (auto next = std::next(i); i->right && (next->level == i->level))
      check_positions(&(*i->right), &(*next));
  }
}

template <class Node> void Tree<Node>::display(const Node *_root) const {

  using node_pos = DisplayNode<T>;

  if (!_root)
    return;

  std::list<node_pos> positions;

  std::queue<const Node *> nodes;

  nodes.push(_root);

  std::queue<node_pos> pos_queue;
  pos_queue.push(node_pos{_root->payload});

  while (!nodes.empty()) {
    const Node *node = nodes.front();
    nodes.pop();

    node_pos pos = pos_queue.front();

    positions.push_back(pos);

    node_pos **child_upd = nullptr;
    if (pos.parent)
      child_upd = (pos.parent->right == &pos_queue.front()) ? &pos.parent->right
                                                            : &pos.parent->left;
    pos_queue.pop();

    if (child_upd)
      *child_upd = &positions.back();

    if (node->left) {
      nodes.push(node->left.get());
      pos_queue.push(node_pos{node->left->payload});
      pos_queue.back().level = pos.level + 1;
      pos_queue.back().position = pos.position - 1 - node_width(*node->left);
      pos_queue.back().parent = &positions.back();
      positions.back().left = &pos_queue.back();
    }

    if (node->right) {
      nodes.push(node->right.get());
      pos_queue.push(node_pos{node->right->payload});
      pos_queue.back().level = pos.level + 1;
      pos_queue.back().position = pos.position + 1 + node_width(*node);
      pos_queue.back().parent = &positions.back();
      positions.back().right = &pos_queue.back();
    }

    for (int level = pos.level; level > 1; level--) {
      check_level(level, positions);
      check_overlaps(positions);
    }
  }

  for (int level = positions.back().level; level > 0; level--) {
    check_level(level, positions);
  }
  check_overlaps(positions);

  int min_pos =
      std::min_element(positions.begin(), positions.end(),
                       [](auto &a, auto &b) { return a.position < b.position; })
          ->position;

  shift_positions(-min_pos, &positions.front());
  draw(positions);
}

template <template <class> class Cont, class T>
void draw(const Cont<DisplayNode<T>> &positions) {
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

    if (pos.left) {
      int ed_pos = pos.left->position + node_width(*pos.left) / 2 + 1;
      lines = pos.position - ed_pos - 1;
      spaces -= lines;
      edges += std::string(ed_pos - edges.length(), ' ') + '/';
    }

    nodes += std::string(spaces, ' ');
    nodes += std::string(lines, '_');
    nodes += pld_str;

    if (pos.right) {
      int ed_pos = pos.right->position + node_width(*pos.right) / 2 - 1;
      lines = ed_pos - nodes.length();
      nodes += std::string(lines, '_');
      edges += std::string(ed_pos - edges.length(), ' ') + '\\';
    }
  }
  std::cout << nodes;
  std::cout << "\n";
}

#endif /* TREE_HPP */
