cmake_minimum_required(VERSION 3.1)

project(b_tree LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

add_executable(b_tree b_tree.cpp)
target_include_directories(b_tree PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(b_tree PRIVATE cxx_std_17)

add_executable(tree_measures measures.cpp)
target_include_directories(tree_measures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(tree_measures PRIVATE cxx_std_17)

add_executable(avl_tree avl.cpp)
target_include_directories(avl_tree PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(avl_tree PRIVATE cxx_std_17)

add_executable(avl_visual avl_visual.cpp)
target_include_directories(avl_visual PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(avl_visual PRIVATE cxx_std_17)

add_executable(avl_vs_bst avl_vs_bst.cpp)
target_include_directories(avl_vs_bst PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(avl_vs_bst PRIVATE cxx_std_17)
