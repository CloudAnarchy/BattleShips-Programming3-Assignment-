cmake_minimum_required(VERSION 3.13)
project(Exercise3_prog3 C)

set(CMAKE_C_STANDARD 11)

find_package (Threads)
add_executable(Exercise3_prog3 main.c structsEnumsDefines.h)
target_link_libraries(Exercise3_prog3 ${CMAKE_THREAD_LIBS_INIT})