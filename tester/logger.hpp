#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <map>
#include <string>
#include <string_view>

class Logger {
public:
  void log(int test_id, std::string_view message);
  void export_logs() const;
private:
  std::map<int, std::string> logs;
};

#endif /* LOGGER_HPP */
