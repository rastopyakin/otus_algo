#include "b_tree.hpp"

#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <utility>
#include <vector>

void search_test();

void display_test();

void remove_visual_test();

void remove_test();

template <class T>
std::pair<BSTree<T>, std::vector<T>>
make_random_tree(int n_elem, std::uniform_int_distribution<T> d) {

  BSTree<int> tree;

  std::mt19937 gen;
  // std::uniform_int_distribution<int> d{0, 100};

  auto seed = std::time(0);
  gen.seed(seed);

  std::vector<int> elems;
  elems.reserve(n_elem);

  std::generate_n(std::back_inserter(elems), n_elem, [&]() { return d(gen); });

  for (const auto &e : elems)
    tree.insert(e);

  return {std::move(tree), elems};
}

int main(int argc, char *argv[]) {

  // display_test();
  search_test();
  // remove_visual_test();
  remove_test();

  return 0;
}

void remove_test() {

  std::cout << "remove test: ";
  bool passed = true;

  int n_elem = 1e6;
  std::uniform_int_distribution<int> d{};
  auto [tree, elems] = make_random_tree<int>(n_elem, d);

  // sort for faster searching
  std::sort(elems.begin(), elems.end());
  elems.erase(std::unique(elems.begin(), elems.end()), elems.end());

  std::vector<int> removed;


  auto predicate = [n = 0, &removed, n_elem](auto e) mutable {
    bool if_removed = n++ % 10 == 0;
    if (if_removed)
      removed.push_back(e);
    return if_removed;
  };

  elems.erase(std::remove_if(elems.begin(), elems.end(), std::ref(predicate)), elems.end());

  for (int e : removed)
    tree.remove(e);

  for (int e : elems)
    if (!tree.search(e)) {
      std::cout << "\nCan't find the element which is not removed\n";
      passed = false;
      break;
    }

  for (int e : removed)
    if (tree.search(e)) {
      std::cout << "\nThe removed element is found\n";
      passed = false;
      break;
    }

  std::cout << (passed ? "OK" : "FAILED") << "\n";

}

void remove_visual_test() {

  int n_elem = 10;

  std::uniform_int_distribution<int> d{0, 100};
  auto [tree, elems] = make_random_tree<int>(n_elem, d);

  tree.display();

  // choose what to remove
  std::mt19937 gen;
  auto seed = std::time(0);
  gen.seed(seed);

  std::uniform_int_distribution<int>::param_type p{0, n_elem - 1};
  d.param(p);
  int ind_to_remove = d(gen);

  std::cout << "\nremove: " << elems[ind_to_remove] << "\n\n";

  tree.remove(elems[ind_to_remove]);
  tree.display();
}

void search_test() {

  std::cout << "search test: ";
  bool passed = true;

  const int n_elem = 1e5;
  std::uniform_int_distribution<int> d{};
  auto [tree, elems] = make_random_tree<int>(n_elem, d);
  // check obviously inserted
  for (auto e : elems)
    if (!tree.search(e)) {
      std::cout << "\nSearching truly inserted failed\n";
      passed = false;
      break;
    }

  // sort for faster searching
  std::sort(elems.begin(), elems.end());

  // check random
  std::mt19937 gen;
  for (int i = 0; i < n_elem; i++) {
    auto e = d(gen);
    if (std::binary_search(elems.begin(), elems.end(), e) != tree.search(e)) {
      std::cout << "\nComparative searching  failed\n";
      passed = false;
      break;
    }
  }

  std::cout << (passed ? "OK" : "FAILED") << "\n";
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