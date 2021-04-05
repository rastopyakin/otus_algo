#include <bits/stdint-uintn.h>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>
#include <tuple>
#include <vector>
#include <regex>

#include "reproc++/drain.hpp"
#include "reproc++/reproc.hpp"

namespace fs = std::filesystem;

class test_files {
public:
  using test_t = std::tuple<fs::path, fs::path, int>;
public:
  test_files(const fs::path &path) {
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
    auto in_file = entry.path();
    auto in_filename = in_file.filename().string();
    auto out_filename = std::regex_replace(in_filename, std::regex{"\\.in"}, ".out");
    auto out_file = entry.path().parent_path() / out_filename;

    if (fs::exists(out_file)) {
      std::smatch match;
      std::regex_search(in_filename,  match, test_num_regex);
      int test_num = std::stoi(match.str());
      return std::make_tuple(in_file, out_file, test_num);
    }
    return std::make_tuple("", "", 0);
  }
};

class tester {
public:
  tester(const std::string &task_name) : task_name(task_name) {}
  void run_tests(const std::vector<test_files::test_t> &files) {

    fs::path in_file, out_file;
    int test_number = 0;
    std::ifstream file;
    std::string input;
    std::string test_output;
    std::stringstream output;
    reproc::sink::ostream sink{output};

    for (const auto &test : files) {

      std::tie(in_file, out_file, test_number) = test;

      file.open(in_file);
      std::getline(file, input);
      input.append("\n");
      file.close();

      reproc::process process;
      process.start(std::vector{task_name});

      std::cout << "running test number " << test_number << "\n";
      process.write(
        reinterpret_cast<const uint8_t *>(input.c_str()), input.size());

      reproc::drain(process, sink, reproc::sink::null);

      file.open(out_file);

      std::string out;
      bool failed = false;

      while (std::getline(file, test_output)) {

        auto pos = test_output.find('\r');
        if (pos != std::string::npos)
          test_output.erase(pos);

        if (std::getline(output, out)) {
          // std::cout << test_output << " vs " << out << "\n";
          if (test_output != out) {
            failed = true;
            break;
          }
        }
        else
          std::cout << "too much data in file\n";
      }

      if (!failed)
        while (std::getline(output, out)) {
          if (!out.empty())
            std::cout << "too much data in task's out\n";
        }

      output.clear();
      output.str(std::string{}); // otherwise output will contain all the previous results
      file.close();

      if (failed) {
        std::cout << "FAILED\n";
        std::cout << "in : " << input;
        std::cout << "out : " << test_output << "\n";
        std::cout << "task's out : " << out << "\n";
      }
      else
        std::cout << "PASSED\n";

      process.wait(reproc::infinite);
    }
  }
private:
  // reproc::process process;
  std::string task_name;
};

int main(int argc, char *argv[]) {

  std::string dir {};

  if (argc == 3)
    dir = argv[2];

  test_files tests {fs::current_path() / dir};

  tester tester {argv[1]};
  tester.run_tests(tests.get_data());

  return 0;
}
