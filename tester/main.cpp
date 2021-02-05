#include <iostream>
#include <filesystem>
#include <system_error>
#include <tuple>
#include <vector>

#include "reproc++/reproc.hpp"
#include "reproc++/run.hpp"

int main(int argc, char *argv[]) {

    reproc::process process;

    int status = 0;
    std::error_code ec;
    reproc::options options;
    options.redirect.parent = true;


    std::tie(status, ec) =  reproc::run(argv + 1, options);

    return 0;
}
