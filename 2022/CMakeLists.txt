project(AdventOfCode2022)

FOREACH (n RANGE 1 3)
    add_executable(2022-day${n} src/day${n}.cpp)
    target_link_libraries(2022-day${n} PRIVATE utils)
ENDFOREACH ()

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
