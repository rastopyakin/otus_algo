#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <tuple>

#include "reproc++/drain.hpp"
#include "reproc++/reproc.hpp"

int main(int argc, char *argv[]) {

  std::ifstream test_file;
  test_file.open("test.0.in");
  std::string input;
  std::getline(test_file, input);
  test_file.close();

  test_file.open("test.0.out");
  std::string test_output;
  std::getline(test_file, test_output);
  test_file.close();

  reproc::process process;

  input.append("\n");
  reproc::options options;

  std::size_t n_bytes = 0;
  std::error_code ec;

  process.start(argv + 1);
  std::tie(n_bytes, ec) = process.write(
    reinterpret_cast<const uint8_t *>(input.c_str()), input.size());

  std::string output;
  reproc::sink::string sink{output};
  reproc::drain(process, sink, reproc::sink::null);

  if (output == test_output)
    std::cout << "PASSED\n";
  else
    std::cout << "FAILED\n";
  return 0;
}
