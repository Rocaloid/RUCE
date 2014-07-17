aux_source_directory(./ SRC_LIST)
aux_source_directorY(../external/cJSON SRC_LIST)

aux_source_directory(../cli CLI_SRC_LIST)

add_library(RUCE SHARED ${SRC_LIST})
add_executable(RUCE_CLI ${CLI_SRC_LIST})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
include_directories(RUCE PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(RUCE CVESVP)
target_link_libraries(RUCE CVEDSP2)
target_link_libraries(RUCE RFNL)
target_link_libraries(RUCE m)

target_link_libraries(RUCE_CLI RUCE)
target_link_libraries(RUCE_CLI CVESVP)
target_link_libraries(RUCE_CLI CVEDSP2)
target_link_libraries(RUCE_CLI RFNL)
target_link_libraries(RUCE_CLI m)

install(FILES RUCE.h
              DESTINATION ${CMAKE_INSTALL_PREFIX}/include/)
install(FILES IO.h
              Roto.h
              Pitchbend.h
              DESTINATION ${CMAKE_INSTALL_PREFIX}/include/RUCE/)

if(CYGWIN OR WIN32)
    message(STATUS "You are compiling this project as COFF.")
    install(TARGETS RUCE LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "You are compiling this project as 64bit.")
    install(TARGETS RUCE LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "You are compiling this project as 32bit.")
    install(TARGETS RUCE LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
else()
    message(FATAL_ERROR "Your system is unsupported!")
endif()

