cmake_minimum_required(VERSION 3.1)

project(7_heap_sort LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

add_executable(heap_sort_test test.cpp)
target_include_directories(heap_sort_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(heap_sort_test PRIVATE ${sorts_SOURCE_DIR})
target_compile_features(heap_sort_test PRIVATE cxx_std_17)

add_executable(heap_sort_measures measures.cpp)
target_include_directories(heap_sort_measures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(heap_sort_measures PRIVATE ${sorts_SOURCE_DIR})
target_compile_features(heap_sort_measures PRIVATE cxx_std_17)
