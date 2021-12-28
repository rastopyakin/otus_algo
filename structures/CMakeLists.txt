cmake_minimum_required(VERSION 3.10)

project(structures LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

add_executable(array_measure array_measure.cpp)
target_compile_options(array_measure PRIVATE -std=c++17)


add_executable(array_test array_test.cpp)
target_compile_options(array_test PRIVATE -std=c++17)

add_executable(matrix_test matrix_test.cpp)
target_compile_options(matrix_test PRIVATE -std=c++17)
