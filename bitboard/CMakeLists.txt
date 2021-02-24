cmake_minimum_required(VERSION 3.1)

project(bitboard LANGUAGES CXX)


add_executable(king king.cpp)
target_compile_features(king PRIVATE cxx_std_17)

add_executable(knight knight.cpp)
target_compile_features(knight PRIVATE cxx_std_17)

add_executable(castle castle.cpp bitboard.cpp)
target_compile_features(castle PRIVATE cxx_std_17)

add_executable(bishop bishop.cpp bitboard.cpp)
target_compile_features(bishop PRIVATE cxx_std_17)

add_executable(queen queen.cpp bitboard.cpp)
target_compile_features(queen PRIVATE cxx_std_17)
