cmake_minimum_required(VERSION 3.30)
project(lab4)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenMP REQUIRED)
include_directories(sqlite)
link_directories(sqlite)

add_executable(lab4 main.cpp)

target_compile_options(lab4 PRIVATE -fopenmp)
target_link_libraries(lab4 PRIVATE -fopenmp sqlite3)