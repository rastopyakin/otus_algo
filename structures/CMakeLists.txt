cmake_minimum_required(VERSION 3.10)

project(structures LANGUAGES CXX)

add_executable(dynamic dynamic.cpp)
target_compile_options(dynamic PRIVATE -std=c++17)
