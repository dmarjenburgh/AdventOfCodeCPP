add_library(utils STATIC
        src/matrix.cpp
        src/IOUtils.cpp
        src/math.cpp
)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
