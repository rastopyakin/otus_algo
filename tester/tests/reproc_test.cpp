#include "reproc++/reproc.hpp"
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <system_error>
#include <thread>
#include <tuple>
#include <utility>

class non_blocking_read {
public:
  using buf_t = std::array<uint8_t, 5>;

public:
  non_blocking_read(reproc::process &process) : _process{process} {
    _thread = std::thread{&non_blocking_read::reader, this};
  }

  ~non_blocking_read() { _thread.join(); }

  template <class T, class P>
  bool read_or_wait_for(std::string &output, std::error_code &ec,
                        const std::chrono::duration<T, P> &dur) {
    std::unique_lock<std::mutex> lk{_m};
    if (_cv.wait_for(lk, dur, [this]() { return !_output.empty() or _ec; })) {
      output = std::move(_output);
      ec = _ec;
      return true;
    }
    output.clear();
    return false;
  }

  std::error_code
  try_read(std::string &output) { // better to return the boolean status
    std::lock_guard<std::mutex> lk{_m};
    output = std::move(_output);
    return _ec;
  }

private:
  void reader() {
    buf_t buf{0};
    std::size_t n_bytes = 0;
    std::error_code ec;
    while (1) {
      std::tie(n_bytes, ec) =
          _process.read(reproc::stream::out, buf.data(), buf.size());
      n_bytes = ec == reproc::error::broken_pipe ? 0 : n_bytes;

      {
        std::lock_guard<std::mutex> l{_m};
        _output.append(reinterpret_cast<const char *>(buf.data()), n_bytes);
        _ec = ec;
      }
      _cv.notify_one();
      if (ec) {
        break;
      }
    }
  }

private:
  std::thread _thread;
  std::mutex _m;
  std::condition_variable _cv;
  reproc::process &_process;

  std::string _output;
  std::error_code _ec{};
};

int main(int argc, char *argv[]) {
  int status = 0;
  std::error_code ec;

  std::cout << "running ";
  for (int i = 1; *(argv + i); i++) {
    std::cout << *(argv + i) << " ";
  }
  std::cout << "\n";

  reproc::process process;

  reproc::options options;
  options.redirect.discard = false;

  options.redirect.parent = false;
  options.redirect.in =
      reproc::redirect{.type = reproc::redirect::type::default_};
  options.redirect.out =
      reproc::redirect{.type = reproc::redirect::type::default_};

  ec = process.start(argv + 1, options);

  non_blocking_read reader{process};
  std::string output;
  std::string input{};

  while (1) {
    std::getline(std::cin, input);
    process.write(reinterpret_cast<const uint8_t *>(input.data()),
                  input.size() * sizeof(std::string::value_type));

    if (std::cin.eof()) {
      process.close(reproc::stream::in);
      // input.clear();
    }

    ec = reader.try_read(output);
    if (!output.empty()) {
      std::cout << output.size() << " read : ";
      std::cout << output << "\n";
    }

    if (ec) {
      std::cout << ec.message() << "\n";
      break;
    }
  }

  reader.read_or_wait_for(output, ec, std::chrono::milliseconds(100));
  std::cout << output.size() << " remaining out : ";
  std::cout << output << "\n";

  std::tie(status, ec) = process.wait(reproc::infinite);

  if (ec)
    std::cout << ec.message() << "\n";

  std::cerr << "exited with status " << status << "\n";
  return 0;
}
