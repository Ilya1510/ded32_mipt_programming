cmake_minimum_required(VERSION 3.7)
project(StrongStack)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp stack.cpp)
add_executable(StrongStack ${SOURCE_FILES})