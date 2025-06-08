cmake_minimum_required(VERSION 3.23)
project(lab5)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)

add_executable(lab5
    lab5.cpp)

target_link_libraries(lab5 PUBLIC MPI::MPI_CXX)