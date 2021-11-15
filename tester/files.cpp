#include "files.hpp"

TestCases::TestCases(const path_t &path) {
  std::smatch match;
  for (const auto &entry : fs::directory_iterator{path}) {
    std::string s = entry.path().filename().string();
    if (fs::is_regular_file(entry) &&
        std::regex_match(s, match, file_regex)) {
      cases.push_back(file_to_case(entry));
    }
  }
}

test_case_t TestCases::file_to_case(const fs::directory_entry &entry) {
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
  return test_case_t{.in = in_file, .out = out_file, .number = test_num};
}
