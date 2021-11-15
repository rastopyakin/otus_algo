#include "reproc++/reproc.hpp"
#include <array>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <ratio>
#include <string>
#include <sys/types.h>
#include <system_error>
#include <thread>
#include <tuple>

void handler(int s) {
  std::cerr << "SIGPIPE is received : " << s << "\n";
  std::exit(1);
}

void register_sigpipe_handler() { std::signal(SIGPIPE, handler); }

void read_and_show(reproc::process &process) {
  static std::array<uint8_t, 512> buff{0};
  auto [n_bytes, ec] =
      process.read(reproc::stream::out, buff.data(), buff.size());
  if (ec) {
    std::cout << ec.message() << "\n";
    n_bytes = 0;
  }
  buff[n_bytes] = 0;
  std::cout << buff.data() << "\n";
}

void write_to(reproc::process &process, std::string str = "FUCK") {
  auto [n_bytes, ec] = process.write(
      reinterpret_cast<const uint8_t *>(str.data()), str.length());
  std::cout << n_bytes << " written\n";
  if (ec)
    std::cout << ec.message() << "\n";
}

int main(int argc, char *argv[]) {
  register_sigpipe_handler();

  reproc::process process;
  reproc::options options;
  // options.redirect.parent = false;
  // options.redirect.in =
  //     reproc::redirect{.type = reproc::redirect::type::pipe};
  // options.redirect.out =
  //     reproc::redirect{.type = reproc::redirect::type::pipe};

  process.start(argv + 1, options);

  write_to(process);
  write_to(process, "zalupa ");
  write_to(process, "FUCK TOY");

  std::thread t {[&process]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    process.close(reproc::stream::in); // send eof
  }};

  std::cout << "polling the process\n";
  int events = 0;
  std::error_code ec{};

  while (1) {
    events = 0;
    std::tie(events, ec) = process.poll(reproc::event::exit |
                                        reproc::event::out | reproc::event::in);

    if (events & reproc::event::exit) {
      std::cout << "exit event\n";
      break;
    }
    if (events & reproc::event::out) {
      std::cout << "out event: ";
      if (ec)
        std::cout << ec.message() << "\n";
      else
        read_and_show(process);
    }
    if (events & reproc::event::in) {
      std::cout << "in event\n";
      write_to(process);
    }
  }

  std::cout << "polling after exit\n";
  std::tie(events, ec) = process.poll(reproc::event::exit);
  if (events & reproc::event::exit) {
    std::cout << "exit event\n";
  }

  t.join();
  return 0;
}
