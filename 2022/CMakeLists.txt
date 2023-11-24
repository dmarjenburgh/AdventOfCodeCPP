project(AdventOfCode2022)

FOREACH (n RANGE 1 1)
    add_executable(2022-day${n} ${PROJECT_SOURCE_DIR}/src/day${n}.cpp)
ENDFOREACH ()

# Link the matrix library to the targets that need them
#target_link_libraries(2021-day9 PRIVATE matrix)

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
