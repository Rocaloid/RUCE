cmake_minimum_required(VERSION 2.8)

project(RFNL)

if(TARGET RFNL)
    return()
endif()

if(NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
endif()

file(GLOB_RECURSE SRC_SOURCES "src/*.c")
add_library(RFNL STATIC ${SRC_SOURCES})
set_target_properties(RFNL PROPERTIES
    C_STANDARD 99
    POSITION_INDEPENDENT_CODE ON
)
target_include_directories(RFNL PRIVATE "src")
target_link_libraries(RFNL m)

add_subdirectory(external/RUtil2)
target_link_libraries(RFNL RUtil2)

# Arrange header files for external use
add_custom_command(TARGET RFNL POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "include/RFNL")
add_custom_command(TARGET RFNL POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Include/RFNL.h" "include/RFNL.h")
file(GLOB_RECURSE SRC_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "src/*.h")
foreach(SRC_HEADER ${SRC_HEADERS})
    add_custom_command(TARGET RFNL POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/${SRC_HEADER}" "include/RFNL/${SRC_HEADER}")
endforeach()
target_include_directories(RFNL INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")  # Test subproject
    include(CTest)
    #add_subdirectory(test)
endif()

# Workaround for CMake version older than 3.1
# Remove this and set the minimum required version to 3.1
# when we make sure everyone is having CMake 3.1
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
