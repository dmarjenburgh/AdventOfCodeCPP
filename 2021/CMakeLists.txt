project(AdventOfCode2021)

set(TARGETS2021 day1 day2 day3 day4 day5 day6 day7 day8)

FOREACH (t ${TARGETS2021})
    add_executable(2021-${t} ${PROJECT_SOURCE_DIR}/src/${t}.cpp)
ENDFOREACH (t)

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
