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
  NodePtr left{nullptr}, right{nullptr};

  std::string display_str() const { return display_str(payload); };

private:
  template <class U> std::string display_str(const U &_payload) const {
    return std::to_string(payload);
  }
  std::string display_str(char c) const { return {c}; }
  const std::string &display_str(const std::string &str) const { return str; }
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

struct DisplayNode : public Node<std::string, DisplayNode, DisplayNode *> {
  // DisplayNode is the tree node with additional info: level, position, parent,
  // and with not owning pointers
  using Base = Node<std::string, DisplayNode, DisplayNode *>;

  DisplayNode(const std::string &str) : Base{str} {}

  int level = 0;
  int position = 0;
  DisplayNode *parent = nullptr;
};

void shift_positions(int shift, DisplayNode *root) {
  // shifts positions of all children of a given subtree
  root->position += shift;
  if (root->left)
    shift_positions(shift, root->left);
  if (root->right)
    shift_positions(shift, root->right);
}

template <class Node> int node_width(const Node &t) {
  return t.display_str().length();
}

void check_positions(DisplayNode &p_l, DisplayNode &p_r) {
  // if to nodes overlap then shift them accordingly with the containing
  // subtree, so they will have enough space between them
  bool need_fix = p_r.position <= p_l.position + node_width(p_l);
  if (need_fix) {

    int shift = p_l.position + node_width(p_l) - p_r.position;

    DisplayNode *root_to_shift = &p_r;
    while (root_to_shift->parent->left == root_to_shift) {
      root_to_shift = root_to_shift->parent;
    }

    shift_positions(shift / 2 + 1, root_to_shift);

    root_to_shift = &p_l;
    while (root_to_shift->parent->right == root_to_shift) {
      root_to_shift = root_to_shift->parent;
    }

    shift_positions(-shift / 2 - 1, root_to_shift);
  }
}

template <template <class> class Cont>
void check_level(int level, Cont<DisplayNode> &positions) {
  // checks the postions of all neigbouring nodes on a given level

  auto is_lvl = [level](auto &pos) { return pos.level == level; };

  auto l_beg = std::find_if(positions.begin(), positions.end(), is_lvl);

  // TODO: potential bug here: l_end will point to smth even if nothing found
  auto l_end = --std::find_if_not(l_beg, positions.end(), is_lvl);

  while (l_beg != l_end) {
    check_positions(*l_beg, *std::next(l_beg));
    l_beg++;
  }
}

template <template <class> class Cont>
void check_overlaps(Cont<DisplayNode> &positions) {
  // checks overlap between a node and its neigbour's child, which would make
  // the tree arcs impossible to draw

  for (auto i = positions.begin(); i != std::prev(positions.end()); i++) {
    if (auto next = std::next(i); next->left && (next->level == i->level))
      check_positions(*i, *next->left);

    if (auto next = std::next(i); i->right && (next->level == i->level))
      check_positions(*i->right, *next);
  }
}

template <template <class> class Cont>
void draw(const Cont<DisplayNode> &positions) {
  int level = positions.front().level;

  std::string nodes, arcs;

  for (const auto &pos : positions) {
    if (level != pos.level) {
      std::cout << nodes << "\n" << arcs << "\n";
      nodes.clear();
      arcs.clear();
      level = pos.level;
    }

    int spaces = pos.position - nodes.length();
    int lines = 0;
    std::string pld_str = pos.display_str();

    if (pos.left) {
      int arc_pos = pos.left->position + node_width(*pos.left) / 2;
      lines = pos.position - arc_pos - 1;
      spaces -= lines;
      arcs += std::string(arc_pos - arcs.length(), ' ') + '/';
    }

    nodes += std::string(spaces, ' ');
    nodes += std::string(lines, '_');
    nodes += pld_str;

    if (pos.right) {
      int arc_pos = pos.right->position + node_width(*pos.right) / 2;
      lines = arc_pos - nodes.length();
      nodes += std::string(lines, '_');
      arcs += std::string(arc_pos - arcs.length(), ' ') + '\\';
    }
  }
  std::cout << nodes;
  std::cout << "\n";
}

template <class Node> void Tree<Node>::display(const Node *_root) const {
  // basically builds the tree with all additional info as it is in DisplayNode, checks
  // the space between nodes upon each node insertion

  // stores the nodes in a list in order of level by level and left to right, because (i)
  // linear container makes the checks easier to implement and (ii) list never reallocates

  if (!_root)
    return;

  std::list<DisplayNode> positions;

  std::queue<const Node *> nodes;

  nodes.push(_root);

  std::queue<DisplayNode> pos_queue;
  pos_queue.push(DisplayNode{_root->display_str()});

  while (!nodes.empty()) {
    const Node *node = nodes.front();
    nodes.pop();

    DisplayNode pos = pos_queue.front();

    positions.push_back(pos);

    if (auto parent = pos.parent; parent) {
      if (parent->left == &pos_queue.front())
        parent->left = &positions.back();
      else
        parent->right = &positions.back();
    }

    pos_queue.pop();

    if (node->left) {
      nodes.push(node->left.get());
      pos_queue.push(DisplayNode{node->left->display_str()});
      pos_queue.back().position = pos.position - node_width(*node->left);
      positions.back().left = &pos_queue.back();

      pos_queue.back().level = pos.level + 1;
      pos_queue.back().parent = &positions.back();
    }

    if (node->right) {
      nodes.push(node->right.get());
      pos_queue.push(DisplayNode{node->right->display_str()});
      pos_queue.back().position = pos.position + node_width(*node);
      positions.back().right = &pos_queue.back();

      pos_queue.back().level = pos.level + 1;
      pos_queue.back().parent = &positions.back();
    }

    for (int level = pos.level; level > 1; level--) {
      check_level(level, positions);
      check_overlaps(positions);
    }
  }

  for (int level = positions.back().level; level > 0; level--) {
    check_level(level, positions);
    check_overlaps(positions);
  }

  int min_pos =
      std::min_element(positions.begin(), positions.end(),
                       [](auto &a, auto &b) { return a.position < b.position; })
          ->position;

  shift_positions(-min_pos, &positions.front());
  draw(positions);
}

#endif /* TREE_HPP */
