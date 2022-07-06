#include "avl.hpp"
#include "b_tree.hpp"
#include "tests.hpp"

int main(int argc, char *argv[]) {
  display_test<AVLTree>(50);
  display_test<BSTree>(50);
  return 0;
}
