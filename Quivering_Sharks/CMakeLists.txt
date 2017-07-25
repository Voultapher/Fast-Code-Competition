cmake_minimum_required(VERSION 3.0)

project(code_comp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(serial serial.cpp)

add_executable(async async.cpp)

