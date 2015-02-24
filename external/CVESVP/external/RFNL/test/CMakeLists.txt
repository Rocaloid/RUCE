aux_source_directory(./TestDFT Test1)
add_executable(TestDFT ${Test1})
target_link_libraries(TestDFT RFNL)
target_link_libraries(TestDFT m)

aux_source_directory(./TestVec Test2)
add_executable(TestVec ${Test2})
target_link_libraries(TestVec RFNL)
target_link_libraries(TestVec m)

aux_source_directory(./TestMath Test3)
add_executable(TestMath ${Test3})
target_link_libraries(TestMath RFNL)
target_link_libraries(TestMath m)

aux_source_directory(./TestInterp Test4)
add_executable(TestInterp ${Test4})
target_link_libraries(TestInterp RFNL)
target_link_libraries(TestInterp m)

aux_source_directory(./TestWindow Test5)
add_executable(TestWindow ${Test5})
target_link_libraries(TestWindow RFNL)
target_link_libraries(TestWindow m)

add_test(TestDFT TestDFT)
add_test(TestVec TestVec)
add_test(TestMath TestMath)
add_test(TestInterp TestInterp)
add_test(TestWindow TestWindow)

get_filename_component(TestDir "." ABSOLUTE)
get_filename_component(TopDir ${CMAKE_SOURCE_DIR} ABSOLUTE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${TestDir}/TestDFT -I${TopDir}/src")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--allow-multiple-definition")

