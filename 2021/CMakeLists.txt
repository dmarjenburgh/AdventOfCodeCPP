project(AdventOfCode2021)

FOREACH (n RANGE 1 20)
    add_executable(2021-day${n} ${PROJECT_SOURCE_DIR}/src/day${n}.cpp)
ENDFOREACH ()

# Link the matrix library to the targets that need them
target_link_libraries(2021-day9 PRIVATE matrix)
target_link_libraries(2021-day11 PRIVATE matrix)
target_link_libraries(2021-day15 PRIVATE matrix)
target_link_libraries(2021-day20 PRIVATE matrix)

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
