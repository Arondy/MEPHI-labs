cmake_minimum_required(VERSION 3.22)
project(lab4b C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab4b
        splay_tree.c
        splay_tree.h
        menu.c
        menu.h
        tree_time.c
        tree_time.h
        input.c
        input.h
        main.c)
