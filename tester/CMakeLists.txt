cmake_minimum_required(VERSION 3.1)

project(tester LANGUAGES CXX)

set(REPROC++ ON)
add_subdirectory(reproc)

find_package(Threads)

add_executable(tester main.cpp problem.cpp files.cpp test.cpp validator.cpp logger.cpp)
target_compile_options(tester PRIVATE -std=c++17)
target_link_libraries(tester reproc++)

add_subdirectory(tests)
