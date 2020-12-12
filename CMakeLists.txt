cmake_minimum_required(VERSION 3.15)
project(JNP_1_task_4)

set(CMAKE_CXX_STANDARD 17)

add_executable(TEST_PRZYTULA tests/computer_tests.cc computer.h)
add_executable(TEST_PECZAR computer_example.cc computer.h)
add_executable(TEST_FB tests/computer_tests_fb.cc computer.h)
add_executable(TEST1 test1.cc computer.h)