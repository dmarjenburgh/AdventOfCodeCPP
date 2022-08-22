project(AdventOfCode2021)

FOREACH (n RANGE 1 14)
    add_executable(2021-day${n} ${PROJECT_SOURCE_DIR}/src/day${n}.cpp)
ENDFOREACH ()

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
