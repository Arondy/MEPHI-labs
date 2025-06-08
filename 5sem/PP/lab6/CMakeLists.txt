cmake_minimum_required(VERSION 3.23)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)

add_compile_options(-Ofast)

add_executable(lab6
    lab6.cpp)

target_link_libraries(lab6 PUBLIC MPI::MPI_CXX)