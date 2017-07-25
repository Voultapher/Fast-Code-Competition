cmake_minimum_required(VERSION 3.8)


project(code_comp)


find_package(Threads REQUIRED)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(serial serial.cpp)
target_link_libraries(serial Threads::Threads)

add_executable(async async.cpp)
target_link_libraries(async Threads::Threads)

target_include_directories(async PRIVATE ${Boost_INCLUDE_DIR})
