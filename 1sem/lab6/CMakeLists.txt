cmake_minimum_required(VERSION 3.22)
project(list C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(list
        list.c
        list.h
        prog.c)

add_compile_definitions(chunk)