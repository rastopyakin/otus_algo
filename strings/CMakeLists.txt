cmake_minimum_required(VERSION 3.1)

project(strings LANGUAGES CXX)


add_executable(strings main.cpp)
target_compile_features(tester PRIVATE cxx_std_17)
