cmake_minimum_required(VERSION 3.1)

project(tester LANGUAGES CXX)


add_executable(tester main.cpp)
target_compile_features(tester PRIVATE cxx_std_17)
