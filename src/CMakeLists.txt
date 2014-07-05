aux_source_directory(./ SRC_LIST)

add_executable(RUCE ${SRC_LIST})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
include_directories(RUCE PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(RUCE CVESVP)
target_link_libraries(RUCE CVEDSP2)
target_link_libraries(RUCE RFNL)
target_link_libraries(RUCE RUtil2)
target_link_libraries(RUCE m)



if(CYGWIN OR WIN32)
    message(STATUS "You are compiling this project as COFF.")
    install(TARGETS RUCE RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "You are compiling this project as 64bit.")
    install(TARGETS RUCE RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "You are compiling this project as 32bit.")
    install(TARGETS RUCE RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
else()
    message(FATAL_ERROR "Your system is unsupported!")
endif()

