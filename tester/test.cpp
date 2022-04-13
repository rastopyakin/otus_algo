#include "test.hpp"
#include "logger.hpp"
#include "problem.hpp"
#include "validator.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <ios>
#include <string>

void Test::run(Problem *problem, const test_case_t &test_case) {
  _id = test_case.number;
  _out = fill(test_case.out);
  _in = fill(test_case.in);

  _problem_out = problem->run(_in);
  _exec_time = problem->execution_time();
  _logger->log(id(), "finished in " + std::to_string(_exec_time) + "s");
}

void Test::check(Validator *validator) { _passed = validator->check(this); }

data_t Test::fill(const path_t &path) {
  data_t data{};
  std::ifstream fs{path, std::ios::in | std::ios::binary};

  fs.seekg(0, std::ios::end);
  size_t fsz = fs.tellg();

  data.reserve(fsz);
  data.resize(fsz);

  fs.seekg(0);
  fs.read(data.data(), fsz);
  fs.close();

  auto last_non_space = std::find_if_not(
      data.crbegin(), data.crend(), [](auto c) { return std::isspace(c); }).base();
  data.erase(last_non_space, data.cend());

  return data;
}
