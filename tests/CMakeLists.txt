cmake_minimum_required(VERSION 3.0)

add_executable(test-signal-segment signal-segment.cpp)
set_target_properties(test-signal-segment PROPERTIES
    CXX_STANDARD 11
    POSITION_INDEPENDENT_CODE ON
)
add_test(test-signal-segment test-signal-segment)
