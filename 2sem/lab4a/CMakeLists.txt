cmake_minimum_required(VERSION 3.22)
project(lab4a C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab4a
        main.c
        binary_tree.c
        binary_tree.h
        input.c
        input.h
        menu.c
        menu.h
        tree_time.c
        tree_time.h
        )
