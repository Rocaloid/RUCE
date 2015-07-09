cmake_minimum_required(VERSION 3.0)
project(libb64)

add_library(b64 STATIC src/cdecode.c src/cencode.c)

set_target_properties(b64 PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(b64 PUBLIC include)
