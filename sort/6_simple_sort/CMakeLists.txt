cmake_minimum_required(VERSION 3.1)

project(6_simple_sort LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

add_executable(simple_sort_test test.cpp)
target_include_directories(simple_sort_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(simple_sort_test PRIVATE ${sorts_SOURCE_DIR})
target_compile_features(simple_sort_test PRIVATE cxx_std_17)

add_executable(simple_sort_measures measures.cpp)
target_include_directories(simple_sort_measures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(simple_sort_measures PRIVATE ${sorts_SOURCE_DIR})
target_compile_features(simple_sort_measures PRIVATE cxx_std_17)

add_executable(upper_bound upper_bound.cpp)
target_include_directories(upper_bound PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(upper_bound PRIVATE ${sorts_SOURCE_DIR})
target_compile_features(upper_bound PRIVATE cxx_std_17)
