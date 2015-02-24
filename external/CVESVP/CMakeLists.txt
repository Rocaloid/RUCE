cmake_minimum_required(VERSION 2.8)

project(CVESVP)

if(TARGET CVESVP)
    return()
endif()

if(NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
endif()

file(GLOB_RECURSE SRC_SOURCES "src/*.c")
add_library(CVESVP STATIC ${SRC_SOURCES})
set_target_properties(CVESVP PROPERTIES
    C_STANDARD 99
    POSITION_INDEPENDENT_CODE ON
)
target_include_directories(CVESVP PRIVATE "src")
target_link_libraries(CVESVP m)

add_subdirectory(external/RUtil2)
add_subdirectory(external/RFNL)
add_subdirectory(external/CVEDSP2)
target_link_libraries(CVESVP RUtil2 RFNL CVEDSP2)

# Arrange header files for external use
add_custom_command(TARGET CVESVP POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "include/CVESVP")
add_custom_command(TARGET CVESVP POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Include/CVESVP.h" "include/CVESVP.h")
file(GLOB_RECURSE SRC_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "src/*.h")
foreach(SRC_HEADER ${SRC_HEADERS})
    add_custom_command(TARGET CVESVP POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/${SRC_HEADER}" "include/CVESVP/${SRC_HEADER}")
endforeach()
target_include_directories(CVESVP INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")  # Test subproject
    include(CTest)
    #add_subdirectory(test)
endif()

# Workaround for CMake version older than 3.1
# Remove this and set the minimum required version to 3.1
# when we make sure everyone is having CMake 3.1
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
