cmake_minimum_required(VERSION 3.1)

project(fibo LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
add_executable(fibo fibo.cpp utils.cpp)

target_compile_features(fibo PRIVATE cxx_std_17)
