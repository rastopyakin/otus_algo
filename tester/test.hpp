#ifndef TEST_HPP
#define TEST_HPP

#include <memory>
#include <string>
#include <string_view>

#include "types.hpp"

class Problem;
class Validator;
class Logger;

class Test {
public:

public:
  Test(Logger *logger) :_logger(logger) {}
  ~Test() = default;
  void run(Problem *problem, const test_case_t &test_file);
  void check(Validator *validator);
  bool passed() const { return _passed; }
  const std::string_view in() const {return data_view_t {_in};}
  const std::string_view out() const {return data_view_t {_out};}
  const std::string_view problem_out() const {return data_view_t {_problem_out};}
  double exec_time() const {return _exec_time;}
  int id() const {return _id;}
private:
  data_t fill(const path_t &path);
private:
  int _id = 0;
  bool _passed = false;
  data_t _in{};
  data_t _out{};
  data_t _problem_out{};
  double _exec_time;
  Logger *_logger;
};

#endif /* TEST_HPP */
