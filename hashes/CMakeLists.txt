cmake_minimum_required(VERSION 3.1)

project(hashes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_executable(hashes_test hashes_test.cpp test.cpp)
target_include_directories(hashes_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(hashes_test PRIVATE cxx_std_17)
