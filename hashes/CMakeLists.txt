cmake_minimum_required(VERSION 3.1)

project(hashes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_executable(hashes_test hash_table.cpp hashes_test.cpp test.cpp)
target_include_directories(hashes_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(hashes_test PRIVATE cxx_std_17)

add_executable(hashes_measures hash_table.cpp hashes_measures.cpp test.cpp)
target_include_directories(hashes_measures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(hashes_measures PRIVATE cxx_std_17)
