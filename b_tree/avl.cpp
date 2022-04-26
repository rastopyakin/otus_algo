#include "avl.hpp"
#include "b_tree.hpp"
#include "tests.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

int main(int argc, char *argv[]) {
  AVLTree<int> tree;
  BSTree<int> b_tree;

  // std::vector<int> nums;
  // std::uniform_int_distribution<int> d{0, 256};
  // std::mt19937 g;
  // g.seed(time(0));

  // int n_elem = 60;
  // auto gen = [&d, &g]() mutable { return d(g); };
  // std::generate_n(std::back_inserter(nums), n_elem, gen);
  // for (int i : nums) {
    // std::cout << i << " ";
    // tree.insert(i);
    // b_tree.insert(i);
  // }
  // std::cout << "\n";

  // b_tree.display();
  // std::cout << "\n";
  // tree.display();

  // tree.check_heights();

  // std::uniform_int_distribution<int> d1 {0, n_elem};

  // std::cout << "deleted: ";
  // for (int i = 0; i < n_elem/3; i++) {
    // tree.display();
    // tree.remove(nums[i]);
    // std::cout << nums[i] << " ";
    // tree.check_heights();
  // }
  // std::cout << "\n";

  // tree.display();
  // tree.check_heights();


  // remove_visual_test<AVLTree>(20);
  search_test<AVLTree>();
  remove_test<AVLTree>();
  return 0;
}
