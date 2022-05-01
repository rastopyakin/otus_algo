#include "avl.hpp"
#include "b_tree.hpp"
#include "tests.hpp"

int main(int argc, char *argv[]) {
  display_test<AVLTree>(45);
  display_test<BSTree>(45);
  return 0;
}
