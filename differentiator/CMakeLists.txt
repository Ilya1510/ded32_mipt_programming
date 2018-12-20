cmake_minimum_required(VERSION 3.7)
project(differentiator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp operation.cpp operation.h parser.cpp parser.h)
add_executable(differentiator ${SOURCE_FILES})