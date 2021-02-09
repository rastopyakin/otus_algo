#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <tuple>
#include <vector>
#include <regex>

#include "reproc++/drain.hpp"
#include "reproc++/reproc.hpp"

namespace fs = std::filesystem;

class test_data {
public:
  using test_t = std::tuple<std::string, std::string, int>;
public:
  test_data(const fs::path &path) {
    std::smatch match;

    for (const auto &entry : fs::directory_iterator {path}) {
     std::string s = entry.path().filename().string();
      if (fs::is_regular_file(entry) &&
          std::regex_match(s, match, file_regex)) {
        tests.push_back(process_entry(entry));
      }
    }
  }
  std::vector<test_t> get_data() const {
    return tests;
  }
private:
  std::vector<test_t> tests;
  const std::regex file_regex {R"(test\.[0-9]+\.in)"};
  const std::regex test_num_regex {R"(\d+)"};

  test_t process_entry(const fs::directory_entry &entry) {
    std::string in_filename = entry.path().filename().string();
    std::string out_filename = std::regex_replace(in_filename, std::regex{"\\.in"}, ".out");
    if (fs::exists(fs::path(out_filename))) {
      std::smatch match;
      std::regex_search(in_filename,  match, test_num_regex);
      int test_num = std::stoi(match.str());
      return std::make_tuple(in_filename, out_filename, test_num);
    }
    return std::make_tuple("", "", 0);
  }
};

int main(int argc, char *argv[]) {

  test_data tests {fs::current_path()};

  for (const auto &test : tests.get_data()) {
    std::string in_file, out_file;
    int num = 0;
    std::tie(in_file, out_file, num) = test;
    std::cout << in_file << " " << out_file << " " << num << "\n";
  }

  std::ifstream test_file;
  test_file.open("test.0.in");
  std::string input;
  std::getline(test_file, input);
  test_file.close();

  test_file.open("test.0.out");
  std::string test_output;
  std::getline(test_file, test_output);
  test_file.close();

  reproc::process process;

  input.append("\n");
  reproc::options options;

  std::size_t n_bytes = 0;
  std::error_code ec;

  process.start(argv + 1);
  std::tie(n_bytes, ec) = process.write(
    reinterpret_cast<const uint8_t *>(input.c_str()), input.size());

  std::string output;
  reproc::sink::string sink{output};
  reproc::drain(process, sink, reproc::sink::null);

  if (output == test_output)
    std::cout << "PASSED\n";
  else
    std::cout << "FAILED\n";

  return 0;
}
