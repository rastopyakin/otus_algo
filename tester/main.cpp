#include <iostream>
#include <string>

#include "files.hpp"
#include "logger.hpp"
#include "problem.hpp"
#include "reproc++/drain.hpp"
#include "reproc++/reproc.hpp"
#include "test.hpp"
#include "validator.hpp"

int main(int argc, char *argv[]) {

  if (argc < 2 or argc > 3) {
    std::cout << "usage: tester name [testdir]\n";
    std::exit(1);
  }

  Problem problem{argv[1]};

  std::string dir{*(argv + 2) ? argv[2] : ""};


  TestCases tests{fs::current_path() / dir};

  Test test{};
  Logger logger;

  Validator validator{&logger};

  for (const auto &test_case : tests.get()) {
    std::cout << "Running test#" << test_case.number << " ...\n";
    test.run(&problem, test_case);
    test.check(&validator);
  }

  logger.export_logs();

  return 0;
}
