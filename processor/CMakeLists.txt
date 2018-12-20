cmake_minimum_required(VERSION 3.7)
project(processor)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp processor.h assembler.h)
add_executable(processor ${SOURCE_FILES})