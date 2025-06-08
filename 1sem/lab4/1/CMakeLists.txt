cmake_minimum_required(VERSION 3.24)
project(1 C)

set(CMAKE_C_STANDARD 11)

add_executable(1
        main.c
        read.c read.h)

add_executable(test test.c)