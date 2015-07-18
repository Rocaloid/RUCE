cmake_minimum_required(VERSION 3.0)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/install/include")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build")

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "${CMAKE_HOST_SYSTEM_PROCESSOR}")
    set(SNDFILE_HOST_ARGS)
else()
    set(SNDFILE_HOST_ARGS "--host=${CMAKE_SYSTEM_PROCESSOR}")
endif()

include(ExternalProject)
ExternalProject_Add(sndfile_builder
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    CONFIGURE_COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/configure" ${SNDFILE_HOST_ARGS} "--prefix=<INSTALL_DIR>" --enable-static --disable-shared --with-pic --disable-alsa --disable-sqlite --disable-external-libs "CC=${CMAKE_C_COMPILER}" "CXX=${CMAKE_CXX_COMPILER}"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/build"
    INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/install"
)
ExternalProject_Add_Step(sndfile_builder copy_source
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/create_symbols_file.py" "${CMAKE_CURRENT_BINARY_DIR}/build/src/create_symbols_file.py"
    DEPENDERS configure
)

add_library(sndfile STATIC IMPORTED GLOBAL)
add_dependencies(sndfile sndfile_builder)
set_target_properties(sndfile PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/install/lib/libsndfile.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/install/include"
)
