project(common)

add_library(${PROJECT_NAME} STATIC
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

