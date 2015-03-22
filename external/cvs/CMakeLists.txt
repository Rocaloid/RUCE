cmake_minimum_required(VERSION 2.8)

project(fnmatch_cvs)

if(TARGET fnmatch_cvs)
    return()
endif()

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /GL")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

add_library(fnmatch_cvs STATIC "fnmatch.c")
set_target_properties(fnmatch_cvs PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(fnmatch_cvs PUBLIC ".")
