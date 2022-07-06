#include "hash_table.hpp"
#include "test.hpp"

int main(int argc, char *argv[]) {

  std::cout << "Simple Rehash HT\n";

  std::cout << "insertion test : ";
  insert_test<HashTable_SimpleRehash>();

  std::cout << "iterating test : ";
  iterator_test<HashTable_SimpleRehash>();

  std::cout << "operator[] test : ";
  operator_access_test<HashTable_SimpleRehash>();

  std::cout << "erase test : ";
  erase_test<HashTable_SimpleRehash>();

  std::cout << "\nPrimesPolicy HT\n";

  std::cout << "insertion test : ";
  insert_test<HashTable_PrimeRehash>();

  std::cout << "iterating test : ";
  iterator_test<HashTable_PrimeRehash>();

  std::cout << "operator[] test : ";
  operator_access_test<HashTable_PrimeRehash>();

  std::cout << "erase test : ";
  erase_test<HashTable_PrimeRehash>();


  return 0;
}
