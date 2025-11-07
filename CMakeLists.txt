cmake_minimum_required(VERSION 3.10)
project(shop-timer C CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF )

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

find_package(Doxygen 1.9 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4>=4.0)
message("GTK4 include dir: ${GTK4_INCLUDE_DIRS}")
message("GTK4 libraries: ${GTK4_LIBRARY_DIRS}")
message("GTK4 other Cflags: ${GTK4_CFLAGS_OTHER}")

pkg_check_modules(SQLITE3 REQUIRED sqlite3)
message("SQLITE3 include dir: ${SQLITE3_INCLUDE_DIRS}")
message("SQLITE3 libraries: ${SQLITE3_LIBRARY_DIRS}")
message("SQLITE3 other Cflags: ${SQLITE3_CFLAGS_OTHER}")

include_directories(
    ${CMAKE_SOURCE_DIR}/src/common
    ${GTK4_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
    )

link_directories(
    ${LIBRARY_OUTPUT_PATH}
    ${GTK4_LIBRARY_DIRS}
    ${SQLITE3_LIBRARY_DIRS}
    )

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    add_compile_options(
        #-g
        -O0
        #-DMEMORY_DEBUG
        #-DUSE_TRACE
        -DUSE_ASSERTS
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "release")
    add_compile_options(
        -Ofast
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "profile")
    add_compile_options(
        -O0
        -pg
    )
endif()

add_compile_options(
    ${GTK4_CFLAGS_OTHER} ${SQLITE3_CFLAGS_OTHER}
    -Wall -Wextra -Wpedantic -pedantic -Wno-extra-semi )


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
