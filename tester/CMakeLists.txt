cmake_minimum_required(VERSION 3.1)

project(tester LANGUAGES CXX)

set(REPROC++ ON)
add_subdirectory(reproc)

add_executable(tester main.cpp)
target_compile_features(tester PRIVATE cxx_std_17)
target_link_libraries(tester reproc++)
