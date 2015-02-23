cmake_minimum_required(VERSION 2.8)

project(RUCE)

if(TARGET RUCE)
    return()
endif()

if(NOT MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -s")
else()
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /GL")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
endif()

include(Version.cmake)
configure_file("src/Config.h.in" "Config.h")

# RUCE.dll
file(GLOB_RECURSE SRC_SOURCES "src/*.c")
add_library(RUCE SHARED ${SRC_SOURCES})
set_target_properties(RUCE PROPERTIES
    C_STANDARD 99  # Requires CMake 3.1
    POSITION_INDEPENDENT_CODE ON
)
target_include_directories(RUCE PRIVATE "src")
target_link_libraries(RUCE m)

add_subdirectory(external/RUtil2)
add_subdirectory(external/CVESVP)
add_subdirectory(external/cJSON)
add_subdirectory(external/cvs)
target_link_libraries(RUCE RUtil2 CVESVP cJSON fnmatch_cvs)

# RUCE.lib
add_library(RUCE_s STATIC ${SRC_SOURCES})
set_target_properties(RUCE_s PROPERTIES
    C_STANDARD 99
    POSITION_INDEPENDENT_CODE ON
)
target_include_directories(RUCE_s PRIVATE "src")
target_link_libraries(RUCE_s m)
target_link_libraries(RUCE_s RUtil2 CVESVP cJSON fnmatch_cvs)

# RUCE_CLI.exe
file(GLOB_RECURSE SRC_CLI_SOURCES "cli/*.c")
add_executable(RUCE_CLI ${SRC_CLI_SOURCES})
set_target_properties(RUCE_CLI PROPERTIES
    C_STANDARD 99
    POSITION_INDEPENDENT_CODE ON
)
target_include_directories(RUCE_CLI PRIVATE "src" "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(RUCE_CLI RUCE_s cJSON)

# Arrange header files for external use
add_custom_command(TARGET RUCE POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "include/RUCE")
add_custom_command(TARGET RUCE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/RUCE.h" "include/RUCE.h")
file(GLOB_RECURSE SRC_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "src/*.h")
foreach(SRC_HEADER ${SRC_HEADERS})
    add_custom_command(TARGET RUCE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/${SRC_HEADER}" "include/RUCE/${SRC_HEADER}")
endforeach()
target_include_directories(RUCE INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")  # Test subproject
    include(CTest)
    #add_subdirectory(test)
endif()

# Installation
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    install(TARGETS RUCE RUCE_CLI
        RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
    )
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include"
        DESTINATION "include"
    )
endif()

# Workaround for CMake version older than 3.1
# Remove this and set the minimum required version to 3.1
# when we make sure everyone is having CMake 3.1
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
