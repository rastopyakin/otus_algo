#ifndef TYPES_HPP
#define TYPES_HPP

#include <filesystem>

namespace fs = std::filesystem;

using path_t = fs::path;
using data_t = std::string;
using data_view_t = std::string_view;

struct test_case_t {
  path_t in{};
  path_t out{};
  int number = 0;
};

#endif /* TYPES_HPP */
