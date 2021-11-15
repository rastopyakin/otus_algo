#ifndef FILES_HPP
#define FILES_HPP

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <regex>
#include <tuple>
#include <vector>

#include "types.hpp"

class TestCases {
public:
  TestCases(const path_t &path);
  const std::vector<test_case_t> &get() const { return cases; }
private:
  std::vector<test_case_t> cases;
  const std::regex file_regex{R"(test\.[0-9]+\.in)"};
  const std::regex test_num_regex{R"(\d+)"};
  test_case_t file_to_case(const fs::directory_entry &entry);
};

#endif /* FILES_HPP */
