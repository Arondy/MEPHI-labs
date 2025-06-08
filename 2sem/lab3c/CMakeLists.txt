cmake_minimum_required(VERSION 3.22)
project(lab3c C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab3c
        hashtable.c
        hashtable.h
        menu.c
        menu.h
        input.c
        input.h
        main.c)
