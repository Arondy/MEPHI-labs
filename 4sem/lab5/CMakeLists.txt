cmake_minimum_required(VERSION 3.22)
project(lab5 C)

include_directories(stb)
set(CMAKE_C_STANDARD 11)

add_executable(lab5 main.c lab.o)
target_link_libraries(lab5 m)
