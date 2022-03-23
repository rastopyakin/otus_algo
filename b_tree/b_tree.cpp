#include "b_tree.hpp"

#include <ctime>
#include <iostream>
#include <random>

int main(int argc, char *argv[]) {

  BSTree<int> tree;

  std::mt19937 gen;
  std::uniform_int_distribution<int> d {0, 10000};

  gen.seed(std::time(0));

  for (int i = 0; i < 100; i++)
    tree.insert(d(gen));

  tree.display();
  return 0;
}
