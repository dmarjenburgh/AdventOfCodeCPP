cmake_minimum_required(VERSION 3.17)
project(AoC)

set(CMAKE_CXX_STANDARD 17)

set(TARGETS2015 day1 day2 day3 day4 day5 day6 day7 day8 day9)
FOREACH (t ${TARGETS2015})
    add_executable(2015-${t} ${PROJECT_SOURCE_DIR}/2015/${t}.cpp)
ENDFOREACH (t)
#add_executable(aoc main.cpp 2015/aoc.h 2015/day2.cpp 2015/day3.cpp 2015/day4.cpp 2015/day5.cpp 2015/day6.cpp 2015/day1.cpp)

# Look in vendor directory for libraries
add_subdirectory(vendor)

# Look in here for include files (header files for #include)
include_directories(vendor/hashlib2plus)

# Link the hashlib2plus library to the targets that need them
target_link_libraries(2015-day4 PRIVATE hashlib2plus)

# Copy assets to build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
