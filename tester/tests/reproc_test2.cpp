#include "reproc++/reproc.hpp"
#include <array>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ratio>
#include <sstream>
#include <system_error>
#include <thread>
#include <tuple>

void handler(int s) {
  std::cerr << "SIGPIPE is received : " << s << "\n";
  std::exit(1);
}

void register_sigpipe_handler() { std::signal(SIGPIPE, handler); }

int main(int argc, char *argv[]) {
  register_sigpipe_handler();
  std::cout << "running ";
  for (int i = 1; *(argv + i); i++) {
    std::cout << *(argv + i) << " ";
  }
  std::cout << "\n";

  reproc::process process;

  process.start(argv + 1, reproc::options{.redirect{.parent = false}});

  std::array<uint8_t, 512> buff = {0};
  int status = 0;
  std::error_code ec;
  std::size_t n_bytes = 0;
  std::string input{"fuck"};

  std::this_thread::sleep_for(std::chrono::milliseconds(3));
  std::cout << "Writing to the process\n";
  int pid = 0;
  std::tie(pid, ec) = process.pid();
  std::cout << "pid : " << pid << "\n";

  std::tie(n_bytes, ec) = process.write(
      reinterpret_cast<const uint8_t *>(input.data()), input.size());
  // if process doesn't take any input the buff will be written anyway if the
  // process still exist; if it doesn't exist (the pipe is closed) the parent
  // receives SIGPIPE

  std::cout << n_bytes << " written\n";
  if (ec)
    std::cout << ec.message();

  std::tie(n_bytes, ec) =
      process.read(reproc::stream::out, buff.data(), buff.size());
  std::cout << n_bytes << " read\n";
  std::cout << "buff : ";
  for (int i = 0; i < n_bytes; i++) {
    if (buff.data()[i] == '\n') {
      std::cout << "__";
      continue;
    }
    std::cout.put(buff.data()[i]);
  }
  std::cout << "\n";

  std::cout << "press enter to run background thread closing streams\n";
  std::cin.get();

  std::thread t{[&process]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    process.close(reproc::stream::in);
  }};

  // auto stop = reproc::options{}.stop;
  std::cout << "waiting for the process to stop\n";
  std::tie(status, ec) = process.wait(reproc::infinite);

  if (ec)
    std::cout << ec.message() << "\n";
  std::cout << "exited with status " << status << "\n";

  t.join();

  // std::cout << "writing to dead process\n";
  // std::tie(n_bytes, ec) = process.write(reinterpret_cast<const
  // uint8_t*>(input.data()), input.size()); std::cout << n_bytes << "
  // written\n"; if (ec) std::cout << ec.message() << "\n";

  return 0;
}
