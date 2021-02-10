#include <exception>
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

class test_files {
public:
  using test_t = std::tuple<std::string, std::string, int>;
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

class tester {
public:
  tester(const std::string &task_name) : task_name(task_name) {}
  void run_tests(const std::vector<test_files::test_t> &filenames) {

    std::string in_file, out_file;
    int test_number = 0;
    std::ifstream file;
    std::string input;
    std::string test_output;
    std::string output;
    reproc::sink::string sink{output};

    for (const auto &test : filenames) {

      std::tie(in_file, out_file, test_number) = test;

      file.open(in_file);
      std::getline(file, input);
      input.append("\n");
      file.close();


      file.open(out_file);
      std::getline(file, test_output);
      file.close();

      reproc::process process;
      process.start(std::vector{task_name});

      std::cout << "running test number " << test_number << "\n";
      process.write(
        reinterpret_cast<const uint8_t *>(input.c_str()), input.size());
      output.clear();           // otherwise output will contain all the previous results
      reproc::drain(process, sink, reproc::sink::null);

      if (output == test_output)
        std::cout << "PASSED\n";
      else {
        std::cout << "FAILED\n";
        std::cout << "in : " << input;
        std::cout << "out : " << test_output << "\n";
        std::cout << "task's out : " << output << "\n";
      }
      process.wait(reproc::infinite);
    }
  }
private:
  // reproc::process process;
  std::string task_name;
};

int main(int argc, char *argv[]) {

  test_files tests {fs::current_path()};

  tester tester {argv[1]};
  tester.run_tests(tests.get_data());

  return 0;
}
