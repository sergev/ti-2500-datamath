#
# Common includes and libraries for all tests.
#
include_directories(BEFORE ../src)
link_libraries(calculator gtest_main)
add_definitions(-DTEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

#
# Check CPU instructions.
#
add_executable(unit_tests # EXCLUDE_FROM_ALL
    keyboard_test.cpp
    arith_test.cpp
)
#add_dependencies(unit_tests ${PROJECT_NAME})
gtest_discover_tests(unit_tests EXTRA_ARGS --gtest_repeat=1 PROPERTIES TIMEOUT 120)
