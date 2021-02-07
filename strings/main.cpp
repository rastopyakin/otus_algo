#include <iostream>
#include <ostream>
#include <string>

int main(int argc, char *argv[]) {

    std::string str;

    std::getline(std::cin, str);
    std::cout << str.size() - 1;
    return 0;
}
