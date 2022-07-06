cmake_minimum_required(VERSION 3.1)

project(kmp LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

add_executable(kmp_test kmp_test.cpp kmp_search.cpp)
target_include_directories(kmp_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kmp_test search_test)

add_executable(kmp_measures kmp_measures.cpp kmp_search.cpp)
# target_compile_options(kmp_measures PRIVATE -lto)
target_include_directories(kmp_measures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kmp_measures search_measures)
