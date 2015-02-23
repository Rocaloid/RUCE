cmake_minimum_required(VERSION 2.8)

project(cJSON)

if(TARGET cJSON)
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

add_library(cJSON STATIC "cJSON.c")
set_target_properties(cJSON PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(cJSON PUBLIC ".")
