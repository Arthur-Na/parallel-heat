cmake_minimum_required(VERSION 3.6)

PROJECT(parallel-heat CUDA CXX)

find_package(VTK REQUIRED)
find_package(OpenMP)
find_package(CUDA)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -pedantic -Wall -Wextra -fopenmp")
set(CMAKE_CXX_STANDARD 17)

include(${VTK_USE_FILE})

add_executable(parallel-heat-view src/main_view.cc src/heat_simulator.cc src/vtk_viewer.cc)
target_link_libraries(parallel-heat-view ${VTK_LIBRARIES})

add_executable(parallel-heat src/main.cc src/heat_simulator.cc src/vtk_viewer.cc)
target_link_libraries(parallel-heat ${VTK_LIBRARIES})

add_executable(parallel-heat-cuda src/main.cu)
