add_library(matrix STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix.cpp)

target_include_directories(matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
