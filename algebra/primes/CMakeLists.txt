cmake_minimum_required(VERSION 3.1)

project(primes LANGUAGES CXX)


add_executable(primes primes.cpp)
target_compile_features(primes PRIVATE cxx_std_17)
