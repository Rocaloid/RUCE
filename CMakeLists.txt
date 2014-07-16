cmake_minimum_required(VERSION 2.8.4)
project(RUCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_definitions(-std=gnu99 -Wall)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions(-march=x86-64 -mtune=generic -mtune=core2 -mtune=core-avx-i)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    add_definitions(-march=i686 -mtune=generic -mtune=core2 -mtune=core-avx-i)
else()
    message(FATAL_ERROR "Your system is unsupported!")
endif()

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-Ofast -DNDEBUG")

set(CMAKE_LD_FLAGS_DEBUG "-Wl,-O0")
set(CMAKE_LD_FLAGS_RELEASE "-Wl,-O1,--as-needed,--sort-common")

add_subdirectory(src)

