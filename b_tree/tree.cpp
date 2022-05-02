#include "tree.hpp"

#include <iostream>

void shift_positions(int shift, DisplayNode *root) {
  // shifts positions of all children of a given subtree
  root->position += shift;
  if (root->left)
    shift_positions(shift, root->left);
  if (root->right)
    shift_positions(shift, root->right);
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
void check_level(int level, std::list<DisplayNode> &positions) {
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
void check_overlaps(std::list<DisplayNode> &positions) {
  // checks overlap between a node and its neigbour's child, which would make
  // the tree arcs impossible to draw

  for (auto i = positions.begin(); i != std::prev(positions.end()); i++) {
    if (auto next = std::next(i); next->left && (next->level == i->level))
      check_positions(*i, *next->left);

    if (auto next = std::next(i); i->right && (next->level == i->level))
      check_positions(*i->right, *next);
  }
}
void draw(const std::list<DisplayNode> &positions) {
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
