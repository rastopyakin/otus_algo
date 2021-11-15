#include "test.hpp"
#include "problem.hpp"
#include "validator.hpp"
#include <cstddef>
#include <fstream>
#include <ios>

void Test::run(Problem *problem, const test_case_t &test_file) {
  _id = test_file.number;
  _out = fill(test_file.out);
  _in = fill(test_file.in);

  _problem_out = problem->run(_in);
}

void Test::check(Validator *validator) {
  _passed = validator->check(this);
}

data_t Test::fill(const path_t &path) {
  data_t data {};
  std::ifstream fs {path, std::ios::in | std::ios::binary};

  fs.seekg(0, std::ios::end);
  size_t fsz = fs.tellg();

  data.reserve(fsz);
  data.resize(fsz);

  fs.seekg(0);
  fs.read(data.data(), fsz);
  fs.close();
  return data;
}
