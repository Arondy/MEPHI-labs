#if (WIN32)
#    add_library(figures SHARED figures.cpp figures.h)
#elseif (UNIX)
#    add_library(figures figures.cpp figures.h)
#endif()

add_library(figures SHARED figures.cpp figures.h)