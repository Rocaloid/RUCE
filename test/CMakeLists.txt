aux_source_directory(./TestRUDB Test1)
add_executable(TestRUDB ${Test1})
target_link_libraries(TestRUDB RUCE)

add_test(TestRUDB TestRUDB)

get_filename_component(TestDir "." ABSOLUTE)
get_filename_component(TopDir ${CMAKE_SOURCE_DIR} ABSOLUTE)

COPY_DIRECTORY_IF_CHANGED("${CMAKE_SOURCE_DIR}/test/TestRUDB/TestData" "${CMAKE_CURRENT_BINARY_DIR}" TestRUDB)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${TestDir}/TestRUDB -I${TopDir}/src")

