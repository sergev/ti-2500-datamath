# Build library
add_library(calculator STATIC
    code.cpp
    cpu.cpp
    disasm.cpp
    keys.cpp
    display.cpp
)

# Build executable file
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} calculator)

# Get git commit hash and revision count
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE GIT_HASH
)
execute_process(
    COMMAND git rev-list HEAD --count
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE GIT_REVCOUNT
)
set_source_files_properties(session.cpp PROPERTIES COMPILE_FLAGS
    -DVERSION_STRING=\\"${CMAKE_PROJECT_VERSION}.${GIT_REVCOUNT}-${GIT_HASH}\\"
)

#install(TARGETS
#    ${PROJECT_NAME}
#    DESTINATION bin
#)
