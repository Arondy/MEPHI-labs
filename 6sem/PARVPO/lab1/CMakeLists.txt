cmake_minimum_required(VERSION 3.29)

project(EquationSolver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление флага для AVX
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")

# Поиск OpenMP
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

# Добавление исполняемого файла
add_executable(EquationSolver main.cpp equation_solver.cpp)

# Указание заголовочных файлов
target_include_directories(EquationSolver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})