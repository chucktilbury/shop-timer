cmake_minimum_required(VERSION 3.10)
project(pgen)

set(CMAKE_VERBOSE_MAKEFILE OFF )

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

find_package(Doxygen 1.9 REQUIRED)

add_subdirectory(src)
add_subdirectory(tests EXCLUDE_FROM_ALL)
add_subdirectory(docs EXCLUDE_FROM_ALL)

file(GLOB_RECURSE format_xxfiles
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.h
)

add_custom_target(format
    COMMENT "Format the source code"
    COMMAND clang-format --verbose -i ${format_xxfiles}
)