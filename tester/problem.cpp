#include "problem.hpp"
#include "reproc++/drain.hpp"
#include "reproc++/reproc.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

data_t Problem::run(data_view_t input) {
  using hrc = std::chrono::high_resolution_clock;

  _exec_time.reset();
  _status.reset();              // reset because new run started

  reproc::process process;
  process.start(std::vector {_args});

  process.write(reinterpret_cast<const uint8_t*>(input.data()), input.size());
  process.close(reproc::stream::in);

  data_t out {};
  reproc::sink::string sink {out};

  auto start = hrc::now();

  reproc::drain(process, sink, sink);
  auto [status, ec] = process.wait(reproc::infinite);

  _exec_time = hrc::now() - start;
  _status = status;

  return out;
}

double Problem::execution_time() const {
  if (!_exec_time)
    throw std::runtime_error{"Execution time hasn't been set"};
  return _exec_time.value().count();
}

int Problem::status() const {
  if (!_status)
    throw std::logic_error{"Exit status hasn't been set"};
  return _status.value();
}
