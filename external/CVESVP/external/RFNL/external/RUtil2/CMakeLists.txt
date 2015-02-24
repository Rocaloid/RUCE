cmake_minimum_required(VERSION 2.8)

project(RUtil2)

if(TARGET RUtil2)
    return()
endif()

if(NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
endif()

include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    add_definitions(-D__BIG_ENDIAN__)
endif()

file(GLOB_RECURSE SRC_SOURCES "src/*.c")
add_library(RUtil2 STATIC ${SRC_SOURCES})
set_target_properties(RUtil2 PROPERTIES
    C_STANDARD 99
    POSITION_INDEPENDENT_CODE ON
)
target_include_directories(RUtil2 PRIVATE "src")
target_link_libraries(RUtil2 m)

# Arrange header files for external use
add_custom_command(TARGET RUtil2 POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "include/RUtil2")
add_custom_command(TARGET RUtil2 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Include/RUtil2.h" "include/RUtil2.h")
file(GLOB_RECURSE SRC_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "src/*.h")
foreach(SRC_HEADER ${SRC_HEADERS})
    add_custom_command(TARGET RUtil2 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/${SRC_HEADER}" "include/RUtil2/${SRC_HEADER}")
endforeach()
target_include_directories(RUtil2 INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")  # Test subproject
    include(CTest)
    #add_subdirectory(test)
endif()

# Workaround for CMake version older than 3.1
# Remove this and set the minimum required version to 3.1
# when we make sure everyone is having CMake 3.1
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
