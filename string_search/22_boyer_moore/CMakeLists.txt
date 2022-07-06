cmake_minimum_required(VERSION 3.1)

project(boyer_moore LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

add_executable(bm_test bm_test.cpp bm_search.cpp)
target_include_directories(bm_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bm_test search_test)

add_executable(bm_test_visual test_visual.cpp bm_search.cpp)
target_include_directories(bm_test_visual PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(bm_test_visual PRIVATE ${string_search_SOURCE_DIR})
target_compile_features(bm_test_visual PRIVATE cxx_std_17)

add_executable(bm_measures bm_measures.cpp bm_search.cpp)
target_include_directories(bm_measures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bm_measures search_measures)
