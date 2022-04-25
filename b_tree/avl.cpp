#include "avl.hpp"
#include "b_tree.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

int main(int argc, char *argv[]) {
  AVLTree<int> tree;
  BSTree<int> b_tree;

  std::vector<int> nums;
  std::uniform_int_distribution<int> d{0, 256};
  std::mt19937 g;
  g.seed(time(0));

  auto gen = [&d, &g]() mutable { return d(g); };
  std::generate_n(std::back_inserter(nums), 60, gen);
  for (int i : nums) {
    std::cout << i << " ";
    tree.insert(i);
    b_tree.insert(i);
  }
  std::cout << "\n";

  b_tree.display();
  std::cout << "\n";
  tree.display();

  // tree.heights();

  return 0;
}
