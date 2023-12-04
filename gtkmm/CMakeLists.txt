cmake_minimum_required(VERSION 3.15)
project(calculator VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig)
pkg_check_modules(GTKMM_VARS REQUIRED IMPORTED_TARGET gtkmm-4.0)

add_executable(calculator
    main_window.cpp
    main.cpp
    ../src/code.cpp
    ../src/cpu.cpp
    ../src/keys.cpp
    ../src/display.cpp
    ../src/trace.cpp
)
target_include_directories(calculator BEFORE PUBLIC
    ../src
)
target_link_libraries(calculator PRIVATE PkgConfig::GTKMM_VARS)
