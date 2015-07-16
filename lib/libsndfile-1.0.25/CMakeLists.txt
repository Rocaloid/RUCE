cmake_minimum_required(VERSION 3.0)

include(ExternalProject)
ExternalProject_Add(sndfile
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    CONFIGURE_COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/configure" --prefix=<INSTALL_DIR> --enable-static --disable-shared --enable-pic --disable-alsa --disable-sqlite --disable-external-libs
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}"
    INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}"
)

find_library(EXTERNAL_PROJECT_SNDFILE sndfile "${CMAKE_CURRENT_BINARY_DIR}/lib")
