#include <iostream>
#include <ostream>
#include <string>

int main(int argc, char *argv[]) {

    std::string str;

    std::getline(std::cin, str);
    auto it = str.find_last_of('\r');
    str.erase(it);
    std::cout << str.length();

    return 0;
}
