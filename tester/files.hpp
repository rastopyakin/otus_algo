#ifndef FILES_HPP
#define FILES_HPP

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <regex>
#include <tuple>
#include <vector>

#include "types.hpp"

class test_files {
public:
  test_files(const fs::path &path) {
    std::smatch match;

    for (const auto &entry : fs::directory_iterator{path}) {
      std::string s = entry.path().filename().string();
      if (fs::is_regular_file(entry) &&
          std::regex_match(s, match, file_regex)) {
        tests.push_back(process_entry(entry));
      }
    }
  }
  const std::vector<test_file_t> &get() const { return tests; }

private:
  std::vector<test_file_t> tests;
  const std::regex file_regex{R"(test\.[0-9]+\.in)"};
  const std::regex test_num_regex{R"(\d+)"};

  test_file_t process_entry(const fs::directory_entry &entry) {
    auto in_file = entry.path();
    auto in_filename = in_file.filename().string();
    auto out_filename =
        std::regex_replace(in_filename, std::regex{"\\.in"}, ".out");
    auto out_file = entry.path().parent_path() / out_filename;

    if (!fs::exists(out_file)) {
      std::cerr << out_filename << "doesn't exist\n";
      std::exit(1);
    }

    std::smatch match;
    std::regex_search(in_filename, match, test_num_regex);
    int test_num = std::stoi(match.str());
    return test_file_t{.in = in_file, .out = out_file, .number = test_num};
  }
};

#endif /* FILES_HPP */
