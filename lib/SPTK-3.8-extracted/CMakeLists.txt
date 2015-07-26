cmake_minimum_required(VERSION 3.0)
project(SPTK)

add_library(SPTK STATIC bin/pitch/snack/jkGetF0.c bin/pitch/snack/sigproc.c bin/nrand/_nrand.c lib/getmem.c)

set_target_properties(SPTK PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Arrange header files for external use
add_custom_command(TARGET SPTK POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "include")
add_custom_command(TARGET SPTK POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/bin/pitch/snack/jkGetF0.pub.h" "include/SPTK.h")
target_include_directories(SPTK INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")
