#include <cstddef>
#include <iostream>

#include "bm_search.hpp"
#include "types.hpp"

int main(int argc, char *argv[]) {

  const char ANSI_RED_BEGIN[] = "\x1B[31m";
  const char ANSI_RED_END[] = "\033[0m";

  std::cout << "text> ";
  string_type text{u8""};
  std::getline(std::cin, text);

  std::cout << "pattern> ";
  string_type pattern{u8""};
  std::getline(std::cin, pattern);

  pos_type pos = 0;
  pos_type cursor_pos = 0;

  pos = v0::boyer_moore_search(text, pattern, pos);

  if (pos == string_type::npos)
    std::cout << "nothing found"
              << "\n";

  string_view_type text_view{text};
  for (; pos != string_type::npos; pos = v0::boyer_moore_horspoole_search(text, pattern, pos + 1)) {

    // std::cout << pos << "\n";
    if (pos > cursor_pos) {
      std::cout << text_view.substr(cursor_pos, pos - cursor_pos);
      cursor_pos += pos - cursor_pos;
    }

    size_t n_already_highlited = cursor_pos - pos > 0 ? cursor_pos - pos : 0;
    std::cout << ANSI_RED_BEGIN
              << text_view.substr(cursor_pos, pattern.length() - n_already_highlited)
              << ANSI_RED_END;

    cursor_pos = cursor_pos + pattern.length() - n_already_highlited;
  }
  std::cout << text_view.substr(cursor_pos) << "\n";

  std::cout << text_view << "\n";

  return 0;
}
