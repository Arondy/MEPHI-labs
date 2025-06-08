cmake_minimum_required(VERSION 3.1)
project(lab3 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab3b
main.c
table.c
table.h
input.c
input.h
menu.c
menu.h)
