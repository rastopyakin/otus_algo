#include "avl.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>

// unfortunately we don't have reflection for enums in C++
enum {INSERT = 0, REMOVE, SEARCH, HELP};
static const char *CMDSTR[] = {"ins", "rm", "src", "help"};

using TREE_NODE_T = int;

template <class T> bool looks_like_command(std::string_view input_to_test) {
  std::stringstream ss;
  ss << input_to_test;
  T maybe_command;

  ss >> maybe_command;

  return ss.fail() || !ss.eof();
}

template <> bool looks_like_command<std::string>(std::string_view s) {
  return std::find(std::begin(CMDSTR), std::end(CMDSTR), s) != std::end(CMDSTR);
}

template <> bool looks_like_command<char>(std::string_view s) {
  return s.length() > 1;
}

int main(int argc, char *argv[]) {

  AVLTree<TREE_NODE_T> tree;

  using HANDLER_T = std::function<void(TREE_NODE_T)>;

  std::map<std::string, HANDLER_T> handlers;

  handlers.insert({CMDSTR[INSERT], [&tree](TREE_NODE_T c) {
    std::cout << "\ninserting " << c << "\n";
    tree.insert(c);
    tree.display();
  }});

  handlers.insert({CMDSTR[REMOVE], [&tree](TREE_NODE_T c) {
    std::cout << "\nremoving: " << c << "\n";
    tree.remove(c);
    tree.display();
  }});

  handlers.insert({CMDSTR[SEARCH], [&tree](TREE_NODE_T c) {
    std::cout << "\nsearching: " << c << "\n";
    std::cout << (tree.search(c) ? "found\n" : "nope\n");
  }});

  HANDLER_T default_handler = handlers["ins"];

  auto help_handler = [&handlers]() {
    std::cout << "possible commands: ";
    const char *comma = "\0, ";
    int i = 0;
    for (const auto &p : handlers) {
      std::cout << comma + i << p.first;
      i = 1;
    }
    std::cout << "\n";
  };

  std::string input;
  std::istringstream iss;

  while (std::getline(std::cin, input)) {
    std::string command;

    iss.clear();
    iss.str(input);

    iss >> command;

    if (looks_like_command<TREE_NODE_T>(command)) {
      if (command == CMDSTR[HELP]) {
        help_handler();
        continue;
      }
      if (handlers.count(command)) {
          default_handler = handlers[command];
      } else {
        std::cout << "No such command: " << command << "\n";
        help_handler();
        continue;
      }
    } else {
      iss.str(input);
      iss.clear();
    }

    TREE_NODE_T c;
    while (iss >> c)
      default_handler(c);
  }
  return 0;
}
