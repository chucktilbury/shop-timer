project(main)

include(${PROJECT_SOURCE_DIR}/../../CMakeBuildOpts.txt)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_files}
)

target_link_libraries(${PROJECT_NAME}
    #gc
    #cord
)
