cmake_minimum_required(VERSION 3.28)
project(curriculum_design)

set(CMAKE_CXX_STANDARD 20)

add_executable(curriculum_design main.cpp)

add_executable(v1.1 v1.1.cpp)

add_executable(test_initTeacher test_initTeacher.cpp)

add_executable(createFileTest createFileTest.cpp)
add_executable(selectTest selectTest.cpp)
