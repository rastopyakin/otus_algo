#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <list>
#include <queue>
#include <algorithm>

#include "node.hpp"

struct DisplayNode : public Node<std::string, DisplayNode, DisplayNode *> {
  // DisplayNode is the tree node with additional info: level, position, parent, and with
  // not owning pointers
  using Base = Node<std::string, DisplayNode, DisplayNode *>;

  DisplayNode(const std::string &str) : Base{str} {}

  int level = 0;
  int position = 0;
  DisplayNode *parent = nullptr;
};

template <class Node> int node_width(const Node &t) {
  return t.display_str().length();
}

// shifts positions of all children of a given subtree
void shift_positions(int shift, DisplayNode *root);

// if to nodes overlap then shift them accordingly with the containing subtree, so they
// will have enough space between them
void check_positions(DisplayNode &p_l, DisplayNode &p_r);

// checks the postions of all neigbouring nodes on a given level
void check_level(int level, std::list<DisplayNode> &positions);

// checks overlap between a node and its neigbour's child, which would make the tree arcs
// impossible to draw
void check_overlaps(std::list<DisplayNode> &positions);

void draw(const std::list<DisplayNode> &positions);

template <class Node> void display_tree(const Node *_root) {
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

#endif /* DISPLAY_HPP */
