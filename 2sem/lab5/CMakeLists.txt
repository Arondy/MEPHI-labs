cmake_minimum_required(VERSION 3.22)
project(lab5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab5
        graph.c
        graph.h
        main.c
        input.c
        input.h
        queue.c
        queue.h
        menu.c
        menu.h)
