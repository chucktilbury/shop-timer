project($ENV{_PROJECT_NAME})

add_executable(${PROJECT_NAME}
    main.c
)

target_link_libraries(${PROJECT_NAME}
    #gc
    #cord
    common
    ${GTK4_LIBRARIES}
    ${SQLITE3_LIBRARIES}
)
