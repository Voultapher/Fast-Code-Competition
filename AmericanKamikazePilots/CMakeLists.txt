cmake_minimum_required(VERSION 3.8)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)

add_executable(app0 src/app0.cc)
target_link_libraries(app0 Threads::Threads)
add_executable(app1 src/app1.cc)
target_link_libraries(app1 Threads::Threads)
add_executable(app2 src/app2.cc)
target_link_libraries(app2 Threads::Threads)
add_executable(app3 src/app3.cc)
target_link_libraries(app3 Threads::Threads)
add_executable(app4 src/app4.cc)
target_link_libraries(app4 Threads::Threads)
