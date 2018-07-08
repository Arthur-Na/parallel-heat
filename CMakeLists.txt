cmake_minimum_required(VERSION 3.6)

PROJECT(parallel-heat CUDA CXX)

enable_language(CUDA)
find_package(VTK REQUIRED)
find_package(OpenMP)
find_package(CUDA)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -pedantic -Wall -Wextra -fopenmp")
#set(CMAKE_CXX_STANDARD 11)

include(${VTK_USE_FILE})

add_executable(parallel-heat-view src/main_view.cc src/heat_simulator.cc src/vtk_viewer.cc)
target_link_libraries(parallel-heat-view ${VTK_LIBRARIES})

add_executable(parallel-heat src/main.cc src/heat_simulator.cc src/vtk_viewer.cc)
target_link_libraries(parallel-heat ${VTK_LIBRARIES})

add_executable(parallel-heat-cuda src/main.cu src/heat_simulator.cu)
set_property(TARGET parallel-heat-cuda PROPERTY CUDA_STANDARD 11)
list(APPEND CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -arch=sm_11")
#set(CUDA_PROPAGATE_HOST_FLAGS ON)
