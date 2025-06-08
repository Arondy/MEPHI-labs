cmake_minimum_required(VERSION 3.24)
project(2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(2
        main.c
        read.c
        read.h)
