#include "logger.hpp"
#include <iostream>

void Logger::log(int test_id, std::string_view message) {
  logs[test_id].append(message);
}

void Logger::export_logs() const {
  for (const auto &log : logs) {
    std::cout << "test #" << log.first << "\n";
    std::cout << log.second << "\n\n";
  }
}
