cmake_minimum_required(VERSION 3.20)
project(compute)

set(CMAKE_CXX_STANDARD 20)

link_directories(/usr/local/lib)
include_directories(.)
include_directories(/usr/local/include)

find_package(cpr REQUIRED)
if (cpr_FOUND)
    message(STATUS "Found cpr: ${cpr_CONFIG} (found version ${cpr_VERSION})")
endif ()

add_compile_options(-lpthread)

add_executable(compute
        crow_all.h
        json.hpp
        compute.cpp)

target_link_libraries(compute cpr)