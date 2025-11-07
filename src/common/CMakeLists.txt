project(common)

include(${PROJECT_SOURCE_DIR}/../../CMakeBuildOpts.txt)

add_library(${PROJECT_NAME} STATIC
    #${${PROJECT_NAME}_files}
    alloc.c
    array.c
    fileio.c
    hash.c
    pointer_list.c
    string_list.c
    string_buffer.c
    trace.c
    cmdline.c
)

