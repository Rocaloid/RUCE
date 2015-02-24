aux_source_directory(./TestOO Test1)
add_executable(TestOO ${Test1})
target_link_libraries(TestOO RUtil2)

aux_source_directory(./TestString Test2)
add_executable(TestString ${Test2})
target_link_libraries(TestString RUtil2)

aux_source_directory(./TestSort Test3)
add_executable(TestSort ${Test3})
target_link_libraries(TestSort RUtil2)

aux_source_directory(./TestIO Test4)
add_executable(TestIO ${Test4})
target_link_libraries(TestIO RUtil2)

aux_source_directory(./TestBase64 Test5)
add_executable(TestBase64 ${Test5})
target_link_libraries(TestBase64 RUtil2)

aux_source_directory(./TestTune Test6)
add_executable(TestTune ${Test6})
target_link_libraries(TestTune RUtil2)

aux_source_directory(./TestCRC32 Test7)
add_executable(TestCRC32 ${Test7})
target_link_libraries(TestCRC32 RUtil2)

add_test(TestOO TestOO)
add_test(TestString TestString)
add_test(TestSort TestSort)
add_test(TestIO TestIO)
add_test(TestBase64 TestBase64)
add_test(TestTune TestTune)
add_test(TestCRC32 TestCRC32)

get_filename_component(TestDir "." ABSOLUTE)
get_filename_component(TopDir ${CMAKE_SOURCE_DIR} ABSOLUTE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${TestDir}/TestOO -I${TestDir}/TestString -I${TestDir}/TestSort -I${TestDir}/TestIO -I${TopDir}/src")

