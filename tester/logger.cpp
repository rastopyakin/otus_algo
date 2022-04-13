#include "logger.hpp"
#include "test.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>

void Logger::log(int test_id, std::string_view message) {
  if (test_id != _last_log_id) {
    std::cout << "\ntest #" << test_id << "\n";
    _last_log_id = test_id;
  }
  std::cout << message << "\n";
}

void Logger::log_exec_time(Test *test) {
  if (test->passed()) {
    times.emplace_back(std::string{test->in()}, test->exec_time());
  }
}

bool Logger::comp_numbers(const std::string &a, const std::string &b) {
  // size_t *l;
  auto a_num = std::stoul(a);
  auto b_num = std::stoul(b);
  return a_num < b_num;
}

void Logger::export_exec_time() {
  // auto &s = times.front().first;
  // auto is_number = [](const auto &v){
  // return std::all_of(v.begin(), v.end, std::isdigit);
  // };

  auto comp_nums = [](auto &a, auto &b) {
    return comp_numbers(a.first, b.first);
  };

  std::sort(std::begin(times), std::end(times), comp_nums);

  auto max_width_it = std::max_element(
      times.begin(), times.end(), [](const auto &a, const auto &b) {
        return a.first.length() < b.first.length();
      });
  size_t max_width = max_width_it->first.length();

  std::ofstream ofs{_log_file, std::ios::out};

  for (const auto &[in, t] : times) {
    ofs << in;
    ofs << std::string(max_width + 4 - in.length(), ' ');
    ofs << t << '\n';
  }
  ofs.close();
}

void Logger::log_failure(Test *test) {
  auto id = test->id();
  log_file_t f_name{"fail." + std::to_string(id) + ".log"};

  log(id, "FAILED");
  log(id, "see " + f_name + " for details");

  std::ofstream ofs{f_name, std::ios::out};
  ofs << "on input\n\t";
  ofs << test->in() << "\n";
  ;
  ofs << "process said\n\t";
  ofs << test->problem_out() << "\n";

  ofs << "instead\n\t";
  ofs << test->out() << "\n";
  ofs.close();
}

void Logger::export_logs() const {
  for (const auto &log : logs) {
    std::cout << "test #" << log.first << "\n";
    std::cout << log.second << "\n";
  }
}
