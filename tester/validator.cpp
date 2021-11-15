#include "validator.hpp"
#include "test.hpp"
#include "logger.hpp"

bool Validator::check(Test *test) {
  auto id = test->id();
  if (test->out() == test->problem_out()) {
    _logger->log(id, "PASSED");
    return true;
  } else {
    _logger->log(id, "FAILED\n");
    _logger->log(id, "on input\n\t");
    _logger->log(id, test->in());
    _logger->log(id, "\nprocess said\n\t");
    _logger->log(id, test->problem_out());
    _logger->log(id, "\ninstead\n\t");
    _logger->log(id, test->out());

    return false;
  }
}
