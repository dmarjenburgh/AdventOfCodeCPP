project(AdventOfCode2022)

add_executable(2022-day1 src/day1.cpp)
target_link_libraries(2022-day1 PRIVATE utils)


# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
