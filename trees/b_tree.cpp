#include "b_tree.hpp"
#include "tests.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  search_test<BSTree>();
  remove_test<BSTree>();

  return 0;
}
