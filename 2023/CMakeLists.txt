project(AdventOfCode2023)

FOREACH (n RANGE 1 6)
    add_executable(2023-day${n} src/day${n}.cpp)
    target_link_libraries(2023-day${n} PRIVATE utils)
ENDFOREACH ()

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
