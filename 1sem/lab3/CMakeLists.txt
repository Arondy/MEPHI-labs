cmake_minimum_required(VERSION 3.23)
project(C99 C)

set(CMAKE_C_STANDARD 99)

add_executable(main "main.c" "menu.c" "ar_func.c")