#include "b_tree.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <vector>

void search_test();

void display_test();

void remove_visual_test();

int main(int argc, char *argv[]) {

  // display_test();
  // search_test();
  remove_visual_test();

  // std::unique_ptr<int> p = std::make_unique<int>(10);

  // auto &p_r = p;

  // std::cout << (p == p_r) << "\n";

  return 0;
}

void remove_visual_test() {
  BSTree<int> tree;

  std::mt19937 gen;
  std::uniform_int_distribution<int> d{0, 100};

  auto seed = std::time(0);
  gen.seed(seed);

  int n_elem = 10;

  std::vector<int> elems;
  elems.reserve(n_elem);

  std::generate_n(std::back_inserter(elems), n_elem, [&]() { return d(gen); });

  for (const auto &e : elems)
    tree.insert(e);

  tree.display();

  // choose what to remove
  std::uniform_int_distribution<int>::param_type p {0, n_elem - 1};
  d.param(p);

  int ind_to_remove = d(gen);

  std::cout << "\nremove: " << elems[ind_to_remove] << "\n\n";

  tree.remove(elems[ind_to_remove]);
  tree.display();
}

void search_test() {

  BSTree<int> tree;

  std::mt19937 gen;
  std::uniform_int_distribution<int> d{};

  const int n_elem = 1e5;
  std::vector<int> elems;
  elems.reserve(n_elem);

  std::generate_n(std::back_inserter(elems), n_elem, [&]() { return d(gen); });

  for (auto e : elems)
    tree.insert(e);

  // check obviously inserted
  for (auto e : elems)
    if (!tree.search(e)) {
      std::cout << "Searching truly inserted failed\n";
      break;
    }

  // sort for faster searching
  std::sort(elems.begin(), elems.end());

  // check random
  for (int i = 0; i < n_elem; i++) {
    auto e = d(gen);
    if (std::binary_search(elems.begin(), elems.end(), e) != tree.search(e)) {
      std::cout << "Comparative searching  failed\n";
      break;
    }
  }
}

void display_test() {
  BSTree<int> tree;

  std::mt19937 gen;
  std::uniform_int_distribution<int> d{};

  auto seed = std::time(0);
  // seed = 1648069972;
  // seed = 1648068713;
  // seed = 1648074155;
  gen.seed(seed);

  std::cout << "seed: " << seed << "\n";

  for (int i = 0; i < 60; i++)
    tree.insert(d(gen));

  tree.display();
}
