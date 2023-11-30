# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.15)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(ti2500 VERSION 0.1 DESCRIPTION "TI-2500 Simulator")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Werror -Wshadow)

#
# TODO: Enable cppcheck by default
#
#find_program(Cppcheck NAMES cppcheck)
#if(NOT (Cppcheck MATCHES "NOTFOUND"))
#    message(STATUS "Found Cppcheck: ${Cppcheck}")
#    set(CMAKE_CXX_CPPCHECK "${Cppcheck}"
#        "--std=c++17"
#        "--enable=style"
#        "--error-exitcode=1" # Fail on any issues
#        "--inline-suppr" # Enable inline control like // cppcheck-suppress "id"
#        "--quiet" # No progress report messages
#        "--suppress=*:*/_deps/*" # Ignore issues in Googletest
#        "--library=${CMAKE_SOURCE_DIR}/test/googletest.xml" # Parse TEST() macro properly
#    )
#endif()

#
# Download GoogleTest
#
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
include(GoogleTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(test)
