if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_library(game_graphics gameRender.cpp gameRender.h)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_library(game_graphics gameRenderRelease.cpp gameRender.h)
endif()

find_package(SFML 2 COMPONENTS network audio graphics window system REQUIRED)
find_package(TGUI 1 REQUIRED)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(game_graphics sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
endif()

if(TGUI_FOUND)
    target_link_libraries(game_graphics TGUI::TGUI)
endif()