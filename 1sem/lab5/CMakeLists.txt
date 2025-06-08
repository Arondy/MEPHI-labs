cmake_minimum_required(VERSION 3.24)
project(lab5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab5
        point.c
        point.h
        prog.c sort.c sort.h)
