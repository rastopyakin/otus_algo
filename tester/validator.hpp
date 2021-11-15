#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

class Test;
class Logger;

class Validator {
public:
  Validator(Logger *logger) :_logger(logger) {}
  ~Validator() = default;
  bool check(Test *test);
private:
  Logger *_logger;
};

#endif /* VALIDATOR_HPP */
