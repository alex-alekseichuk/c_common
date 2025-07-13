cmake_minimum_required(VERSION 3.15)
project(example C)

add_subdirectory(../c_lib_template common_build)

SET(SRC
    src/main.c
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE common)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:common>  # Path to DLL
        $<TARGET_FILE_DIR:example>  # Where to copy it
)
