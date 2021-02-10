cmake_minimum_required(VERSION 3.1)

project(tickets LANGUAGES CXX)


add_executable(tickets main.cpp)
target_compile_features(tickets PRIVATE cxx_std_17)
