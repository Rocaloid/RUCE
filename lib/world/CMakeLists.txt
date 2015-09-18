cmake_minimum_required(VERSION 3.0)
project(world-wav)

add_library(world-wav STATIC matlabfunctions.c)

set_target_properties(world-wav PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Arrange header files for external use
add_custom_command(TARGET world-wav POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "include")
add_custom_command(TARGET world-wav POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/matlabfunctions.h" "include/matlabfunctions.h")
target_include_directories(world-wav INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")
