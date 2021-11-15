#include "problem.hpp"
#include "reproc++/drain.hpp"
#include "reproc++/reproc.hpp"
#include <cstdint>
#include <vector>

data_t Problem::run(data_view_t input) {
  reproc::process process;
  process.start(std::vector {_args});

  process.write(reinterpret_cast<const uint8_t*>(input.data()), input.size());
  process.close(reproc::stream::in);

  data_t out {};
  reproc::sink::string sink {out};

  reproc::drain(process, sink, sink);

  process.wait(reproc::infinite);

  return out;
}
