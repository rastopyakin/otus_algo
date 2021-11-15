#include "reproc++/reproc.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  std::ostringstream os;
  uint64_t result = 0;
  for (uint64_t i = 0; i < 100'000'000; i++) {
    result += i;
    os << i << ' ';
  }
  std::cout << "stream size : " << os.str().size()/1024/1024 << " Mbytes\n";
  std::cout << "result should be  : " << result << "\n";

  reproc::process process;
  process.start(argv + 1);

  process.write(reinterpret_cast<const uint8_t *>(os.str().data()),
                os.str().length());
  process.close(reproc::stream::in);

  auto [status, ec] =
      process.wait(reproc::infinite); // wait for the process to finish
  std::cout << "finished with status " << status << "\n";

  std::string output{};
  std::array<uint8_t, 5> buff;

  while (1) {
    auto [bytes_read, ec] =
        process.read(reproc::stream::out, buff.data(), buff.size());
    if (ec && ec != reproc::error::broken_pipe)
      break;

    bytes_read = ec == reproc::error::broken_pipe ? 0 : bytes_read;
    output.append(reinterpret_cast<const char *>(buff.data()), bytes_read);
    if (ec) {
      std::cout << ec.message() << "\n";
      break;
    }
  }

  std::cout << output << "\n";

  return 0;
}
