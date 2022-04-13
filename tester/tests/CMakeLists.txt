cmake_minimum_required(VERSION 3.10)

project(reproc_tests LANGUAGES CXX)

add_executable(reproc_test reproc_test.cpp)
target_compile_options(reproc_test PRIVATE -std=c++17)
target_link_libraries(reproc_test reproc++)

add_executable(reproc_test2 reproc_test2.cpp)
target_compile_options(reproc_test2 PRIVATE -std=c++17)
target_link_libraries(reproc_test2 reproc++ Threads::Threads)

add_executable(reproc_test3 reproc_test3.cpp)
target_compile_options(reproc_test3 PRIVATE -std=c++17)
target_link_libraries(reproc_test3 reproc++ Threads::Threads)

add_executable(reproc_poll reproc_poll.cpp)
target_compile_options(reproc_poll PRIVATE -std=c++17)
target_link_libraries(reproc_poll reproc++ Threads::Threads)

add_executable(test3 test3.cpp)
add_executable(cat cat.cpp)
add_executable(str_equal str_equal.cpp)
target_compile_options(str_equal PRIVATE -std=c++17)
