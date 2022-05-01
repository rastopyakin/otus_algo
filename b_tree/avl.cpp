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

  // remove_visual_test<AVLTree>(20);
  search_test<AVLTree>();
  remove_test<AVLTree>();
  // display_test<AVLTree>();
  // display_test<BSTree>();
  return 0;
}
