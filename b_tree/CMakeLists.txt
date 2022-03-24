cmake_minimum_required(VERSION 3.1)

project(b_tree LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

add_executable(b_tree b_tree.cpp)
target_include_directories(b_tree PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(b_tree PRIVATE cxx_std_17)
