cmake_minimum_required(VERSION 3.22)
project(lab1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab1
        main.cpp
        matrix.cpp
        matrix.h
        menu.cpp
        menu.h)