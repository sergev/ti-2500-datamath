cmake_minimum_required(VERSION 3.15)
project(wxcalc VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

add_executable(calculator
    application.cpp
    main_window.cpp
)
target_link_libraries(calculator ${wxWidgets_LIBRARIES})
