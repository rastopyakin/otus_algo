#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <chrono>
#include <optional>

#include "types.hpp"

class Problem {
public:
  Problem(const std::string &args) : _args{args} {}
  ~Problem() = default;
  data_t run(data_view_t input);
  double execution_time() const;
  int status() const;
private:
  std::string _args;
  std::optional<std::chrono::duration<double>> _exec_time;
  std::optional<int> _status;
};

#endif /* PROBLEM_HPP */
