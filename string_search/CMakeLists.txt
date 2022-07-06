cmake_minimum_required(VERSION 3.1)

project(string_search LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_library(utf8_tools SHARED utf8_tools.cpp)
target_include_directories(utf8_tools PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(utf8_tools PRIVATE cxx_std_17)

add_library(search_test SHARED test.cpp full_search.cpp)
target_link_libraries(search_test utf8_tools)
target_include_directories(search_test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(search_test PUBLIC cxx_std_17)

add_library(search_measures SHARED measures.cpp)
target_link_libraries(search_measures utf8_tools)
target_include_directories(search_measures INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(search_measures PUBLIC cxx_std_17)


add_executable(full_search_test full_search_test.cpp)
target_link_libraries(full_search_test search_test)

add_executable(full_search_measures full_search_measures.cpp full_search.cpp)
target_link_libraries(full_search_measures search_measures)
target_include_directories(full_search_measures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(unigen unigen.cpp)
target_link_libraries(unigen utf8_tools)
target_include_directories(unigen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(unigen PRIVATE cxx_std_17)

add_executable(utf8_parse utf8_parse.cpp)
target_link_libraries(utf8_parse utf8_tools)
target_include_directories(utf8_parse PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(utf8_parse PRIVATE cxx_std_17)

add_subdirectory(22_boyer_moore)
add_subdirectory(24_kmp)
