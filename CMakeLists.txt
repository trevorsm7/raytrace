cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(raytrace CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

set(SOURCE_FILES
    source/main.cpp
    source/Camera.cpp
    source/Material.cpp
    source/Object.cpp
    source/ppm.cpp
    source/Sampler.cpp)

add_executable(raytrace ${SOURCE_FILES})
target_include_directories(raytrace PRIVATE extern)
