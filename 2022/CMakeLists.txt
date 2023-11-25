project(AdventOfCode2022)

add_executable(2022-day1 src/day1.cpp)
add_executable(2022-day2 src/day2.cpp)

target_link_libraries(2022-day1 PRIVATE utils)
target_link_libraries(2022-day2 PRIVATE utils)

# Copy assets to build directory
file(COPY assets DESTINATION ${PROJECT_BINARY_DIR})
