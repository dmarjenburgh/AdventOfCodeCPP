cmake_minimum_required(VERSION 3.17)
project(aoc)

set(CMAKE_CXX_STANDARD 17)

add_executable(aoc main.cpp 2015/aoc.h 2015/day2.cpp 2015/day3.cpp 2015/day4.cpp 2015/day5.cpp 2015/day6.cpp)

# Look in vendor directory for libraries
add_subdirectory(vendor)

# Look in here for include files (header files for #include)
include_directories(vendor/hashlib2plus)

# Link the hashlib2plus library when building the aoc executable
target_link_libraries(aoc PRIVATE hashlib2plus)

# Copy assets to build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
