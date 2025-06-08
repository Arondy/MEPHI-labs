cmake_minimum_required(VERSION 3.22)
project(lab3)
set(CMAKE_CXX_STANDARD 20)

# dll'ки должны лежать рядом с бинарником
if (WIN32)
    set(MyLib "figures")
    add_library("${MyLib}" SHARED IMPORTED)

    set_property(TARGET "${MyLib}" PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/libs/lib${MyLib}.dll")
    set_property(TARGET "${MyLib}" PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_BINARY_DIR}/libs/lib${MyLib}.dll.a")
endif()

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
    set(ICON_RC "${CMAKE_CURRENT_BINARY_DIR}/icon.rc")
    file(WRITE ${ICON_RC} "1 ICON \"${CMAKE_CURRENT_BINARY_DIR}/textures/icon.ico\"")
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT
            "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
endif()

#add_subdirectory(figures)
add_subdirectory(game_graphics)
add_subdirectory(game_state)
add_subdirectory(game_components)
add_subdirectory(vector)

if (UNIX)
    add_subdirectory(tests)
endif()

set_target_properties(game_components PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(vector PROPERTIES LINKER_LANGUAGE CXX)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(${PROJECT_NAME} mainRelease.cpp "${CMAKE_CURRENT_BINARY_DIR}/icon.rc")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${PROJECT_NAME} main.cpp)
endif()

target_link_libraries(${PROJECT_NAME} game_graphics game_state "${MyLib}" game_components vector)