#include "b_tree.hpp"
#include "tests.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  // display_test<BSTree>();
  search_test<BSTree>();
  // remove_visual_test<BSTree>();
  remove_test<BSTree>();

  // BSTree<int> tree;
  // tree.insert(10);
  // tree.insert(5);
  // tree.insert(15);
  // std::cout << tree.search(11) << "\n";
  // std::cout << tree.search(15) << "\n";

  // tree.remove(11);

  // tree.display();

  return 0;
}
