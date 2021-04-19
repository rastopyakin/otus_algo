cmake_minimum_required(VERSION 3.1)

project(power LANGUAGES CXX)


add_executable(power power.cpp)
target_compile_features(power PRIVATE cxx_std_17)
