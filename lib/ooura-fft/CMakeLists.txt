cmake_minimum_required(VERSION 3.0)
project(ooura-fft)

add_library(ooura-fft STATIC fftsg.c)

set_target_properties(ooura-fft PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Arrange header files for external use
add_custom_command(TARGET ooura-fft POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "include")
add_custom_command(TARGET ooura-fft POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/fftsg.pub.h" "include/ooura-fft.h")
target_include_directories(ooura-fft INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")
