cmake_minimum_required(VERSION 3.6)

PROJECT(parallel-heat)

find_package(VTK REQUIRED)
find_package(OpenMP)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -pedantic -Wall -Wextra -fopenmp")
set(CMAKE_CXX_STANDARD 17)

include(${VTK_USE_FILE})

add_executable(parallel-heat-view src/main_view.cc src/heat_simulator.cc src/vtk_viewer.cc)
target_link_libraries(parallel-heat-view ${VTK_LIBRARIES})

add_executable(heat_diffusion src/main.cc src/heat_simulator.cc src/vtk_viewer.cc)
target_link_libraries(heat_diffusion ${VTK_LIBRARIES})
