if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    if(NOT DEFINED WITH_IN_SOURCE_BUILD)
        message(FATAL_ERROR
            "CMake generation for this project is not allowed within the source directory!"
            "\n Remove the CMakeCache.txt file and try again from another folder, e.g.:"
            "\n "
            "\n rm -r CMakeFiles CMakeCache.txt"
            "\n cd .."
            "\n mkdir RUCE-make"
            "\n cd RUCE-make"
            "\n cmake ../RUCE"
            "\n "
            "\n Alternately define WITH_IN_SOURCE_BUILD to force this option (not recommended!)"
        )
    endif()
endif()
cmake_minimum_required(VERSION 2.8.4)
project(RUCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_definitions(-std=gnu99 -Wall)

# For components upper than CVESVP, we should not specify the platform to ensure
#   portability.
message("-- Detected Supported Platform: " ${CMAKE_SYSTEM_PROCESSOR})

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-Ofast -DNDEBUG")

set(CMAKE_LD_FLAGS_DEBUG "-Wl,-O0")
set(CMAKE_LD_FLAGS_RELEASE "-Wl,-O1,--as-needed,--sort-common")

include(GNUInstallDirs)

# Uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    add_definitions(-D__BIG_ENDIAN__)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/FileCopy.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/Version.cmake)


enable_testing()

add_subdirectory(src)
add_subdirectory(test)

configure_file(
"${CMAKE_CURRENT_SOURCE_DIR}/src/Config.h.in"
"${PROJECT_BINARY_DIR}/src/Config.h"
)

