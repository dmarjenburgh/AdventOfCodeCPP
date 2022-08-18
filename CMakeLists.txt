cmake_minimum_required(VERSION 3.17)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(2015)
add_subdirectory(2021)

# Look in vendor directory for libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor ${CMAKE_BINARY_DIR}/vendor)
