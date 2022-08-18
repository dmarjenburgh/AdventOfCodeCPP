project(AdventOfCode2015)

set(TARGETS2015 day1 day2 day3 day4 day5 day6 day7 day8 day9 day10 day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 day21 day22)

FOREACH (t ${TARGETS2015})
    add_executable(2015-${t} ${PROJECT_SOURCE_DIR}/src/${t}.cpp)
ENDFOREACH (t)

# Link the hashlib2plus library to the targets that need them
target_link_libraries(2015-day4 PRIVATE hashlib2plus)

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
