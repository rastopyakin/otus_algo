#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include "types.hpp"

class Problem {
public:
  Problem(const std::string &args) : _args{args} {}
  ~Problem() = default;
  data_t run(data_view_t input);
private:
  std::string _args;
};

#endif /* PROBLEM_HPP */
