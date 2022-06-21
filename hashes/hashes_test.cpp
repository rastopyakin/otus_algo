#include "hash_table.hpp"
#include "test.hpp"


int main(int argc, char *argv[]) {

  std::cout << "insertion test : ";
  insert_test<HashTable>();

  std::cout << "iterating test : ";
  iterator_test<HashTable>();

  std::cout << "operator[] test : ";
  operator_access_test<HashTable>();

  std::cout << "erase test : ";
  erase_test<HashTable>();

  return 0;
}
