#include "b_tree.hpp"
#include "tests.hpp"

int main(int argc, char *argv[]) {

  // display_test<BSTree>();
  search_test<BSTree>();
  // remove_visual_test<BSTree>();
  remove_test<BSTree>();

  return 0;
}
