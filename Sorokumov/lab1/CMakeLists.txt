cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 14)
add_library(l SHARED src/lab1.cpp)
add_executable(lab1 src/lab1.cpp)


