cmake_minimum_required(VERSION 3.1)

project(otus_algo LANGUAGES CXX)
add_compile_options(-Wall)

add_subdirectory(tester)
add_subdirectory(strings)
add_subdirectory(tickets)
add_subdirectory(bitboard)
add_subdirectory(algebra)
add_subdirectory(structures)
add_subdirectory(b_tree)
# enable_testing()
