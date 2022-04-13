#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <map>
#include <string>
#include <string_view>
#include <vector>

class Test;

class Logger {
public:
  using log_file_t = std::string;

  void log(int test_id, std::string_view message);
  void log_failure(Test *test);
  void log_exec_time(Test *test);
  void export_exec_time();
  void export_logs() const;
private:
  log_file_t _log_file {"perf.log"};
  std::map<int, std::string> logs;
  std::vector<std::pair<std::string, double>> times;
  int _last_log_id = -1;
private:
  static bool comp_numbers(const std::string &a, const std::string &b);
};

#endif /* LOGGER_HPP */
