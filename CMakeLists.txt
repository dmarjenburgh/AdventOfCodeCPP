cmake_minimum_required(VERSION 3.17)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Util library for common tasks and data structures
add_subdirectory(${CMAKE_SOURCE_DIR}/lib)

# Look in vendor directory for libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor ${CMAKE_BINARY_DIR}/vendor)

add_subdirectory(2015)
add_subdirectory(2021)
add_subdirectory(2022)
add_subdirectory(2023)
