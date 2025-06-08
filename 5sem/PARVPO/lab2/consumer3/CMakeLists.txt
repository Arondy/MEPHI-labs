cmake_minimum_required(VERSION 3.20)
project(consumer)

set(CMAKE_CXX_STANDARD 20)

link_directories(/usr/local/lib)
include_directories(.)
include_directories(/usr/local/include)

add_compile_options(-lpthread)

find_package(cpr REQUIRED)
if (cpr_FOUND)
    message(STATUS "Found cpr: ${cpr_CONFIG} (found version ${cpr_VERSION})")
endif ()

add_executable(consumer
        crow_all.h
        json.hpp
        consumer.cpp)

target_link_libraries(consumer cpr)