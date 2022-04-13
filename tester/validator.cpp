#include "validator.hpp"
#include "logger.hpp"
#include "test.hpp"

#include <algorithm>

bool Validator::check(Test *test) {
  auto id = test->id();
  if (validate(test)) {
    _logger->log(id, "PASSED");
    return true;
  } else {
    _logger->log_failure(test);
    return false;
  }
}

void Validator::log_failure(Test *test) {}

bool Validator::validate(Test *test) {
  auto a_it = test->out().cbegin();
  auto b_it = test->problem_out().cbegin();

  while (a_it != test->out().cend() and b_it != test->problem_out().cend()) {
    std::tie(a_it, b_it) = std::mismatch(a_it, test->out().cend(), b_it);
    while (std::isspace(*a_it))
      a_it++;
    while (std::isspace(*b_it))
      b_it++;
    if (*a_it != *b_it)
      return false;
  }
  return true;
  // return test->out() == test->problem_out();
}
