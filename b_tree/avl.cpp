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

  search_test<AVLTree>();
  remove_test<AVLTree>();

  return 0;
}
