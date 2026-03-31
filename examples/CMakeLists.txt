cmake_minimum_required(VERSION 3.15)

function(copy_dll target dll_target)
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${dll_target}>
                $<TARGET_FILE_DIR:${target}>
            COMMENT "Copying ${dll_target}.dll to ${target} directory"
        )
    endif()
endfunction()

add_executable(print_tree src/print_tree.c)
target_link_libraries(print_tree PRIVATE ${PROJECT_NAME})

copy_dll(print_tree common)
