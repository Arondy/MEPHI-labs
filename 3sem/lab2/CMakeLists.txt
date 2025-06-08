cmake_minimum_required(VERSION 3.21)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_subdirectory(cards)
add_subdirectory(tests)

add_executable(lab2 main.cpp menu.cpp menu.h)

target_link_libraries(lab2 cards cards_pile)