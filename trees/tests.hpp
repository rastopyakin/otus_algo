#ifndef TESTS_HPP
#define TESTS_HPP

#include <type_traits>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

template <template<class> class Tree>
std::pair<Tree<int>, std::vector<int>>
make_random_tree(int n_elem, std::uniform_int_distribution<int> d) {

  Tree<int> tree;

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

template <class Tree, class U = int>
struct has_check_heights : std::false_type {};

template <class Tree>
struct has_check_heights<Tree, decltype(Tree{}.check_heights(), int{})> : std::true_type {};

template <class Tree>
constexpr bool has_check_heights_v = has_check_heights<Tree>::value;

template <template<class> class Tree>
void search_test() {

  std::cout << "search test: ";
  bool passed = true;

  const int n_elem = 1e5;
  std::uniform_int_distribution<int> d{};
  auto [tree, elems] = make_random_tree<Tree>(n_elem, d);

  if constexpr (has_check_heights_v<Tree<int>>) {
    tree.check_heights();
  }

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

template <template<class> class Tree>
void remove_test() {

  std::cout << "remove test: ";
  bool passed = true;

  int n_elem = 1e6;
  std::uniform_int_distribution<int> d{};
  auto [tree, elems] = make_random_tree<Tree>(n_elem, d);

  // sort for faster searching
  std::sort(elems.begin(), elems.end());
  elems.erase(std::unique(elems.begin(), elems.end()), elems.end());

  std::vector<int> removed;

  auto predicate = [n = 0, &removed](auto e) mutable {
    bool if_removed = n++ % 5 == 0;
    if (if_removed)
      removed.push_back(e);
    return if_removed;
  };

  elems.erase(std::remove_if(elems.begin(), elems.end(), std::ref(predicate)), elems.end());

  for (int e : removed)
    tree.remove(e);

  if constexpr (has_check_heights_v<Tree<int>>) {
    tree.check_heights();
  }

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

template <template<class> class Tree>
void display_test(int n_elem = 60) {
  Tree<int> tree;

  std::mt19937 gen;
  std::uniform_int_distribution<int> d{0, 1000};

  auto seed = std::time(0);

  gen.seed(seed);

  std::cout << "seed: " << seed << "\n";

  for (int i = 0; i < n_elem; i++)
    tree.insert(d(gen));

  tree.display();
}

template <template<class> class Tree>
void remove_visual_test(int n_elem = 10) {

  std::uniform_int_distribution<int> d{0, 100};
  auto [tree, elems] = make_random_tree<Tree>(n_elem, d);

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

#endif /* TESTS_HPP */
