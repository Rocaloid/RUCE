cmake_minimum_required(VERSION 3.0)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/install/build")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/install/include")

include(ExternalProject)
ExternalProject_Add(sndfile_builder
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    CONFIGURE_COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/configure" "--host=${CMAKE_SYSTEM_PROCESSOR}" "--prefix=<INSTALL_DIR>" --enable-static --disable-shared --with-pic --disable-alsa --disable-sqlite --disable-external-libs "CC=${CMAKE_C_COMPILER}" "CXX=${CMAKE_CXX_COMPILER}"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/build"
    INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/install"
)

add_library(sndfile STATIC IMPORTED GLOBAL)
add_dependencies(sndfile sndfile_builder)
set_target_properties(sndfile PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/install/lib/libsndfile.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/install/include"
)
