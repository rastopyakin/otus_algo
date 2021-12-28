cmake_minimum_required(VERSION 3.10)

project(structures LANGUAGES CXX)

add_executable(dynamic dynamic.cpp)
target_compile_options(dynamic PRIVATE -std=c++17)

add_executable(find_alg find_alg.cpp)
target_compile_options(find_alg PRIVATE -std=c++17)

set(CMAKE_BUILD_TYPE Debug)

add_executable(array_test array_test.cpp)
target_compile_options(array_test PRIVATE -std=c++17)

add_executable(matrix_test matrix_test.cpp)
target_compile_options(matrix_test PRIVATE -std=c++17)
