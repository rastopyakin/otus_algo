#include "avl.hpp"
#include "tests.hpp"

int main(int argc, char *argv[]) {

  search_test<AVLTree>();
  remove_test<AVLTree>();

  return 0;
}
