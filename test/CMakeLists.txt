cmake_minimum_required(VERSION 3.15)

# Find test framework
find_package(unity REQUIRED)

# Collect test source files
file(GLOB TEST_FILES
    src/test_*.c
)

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

foreach(TEST_FILE ${TEST_FILES})
    # get test name without path and extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # create executable
    add_executable(${TEST_NAME} ${TEST_FILE} src/allocator.c src/stack.c)

    # link unity and project library
    target_link_libraries(${TEST_NAME}
        PRIVATE
            unity::unity
            ${PROJECT_NAME}
    )

    # add_custom_command(TARGET ${TEST_NAME} POST_BUILD
    #     COMMAND ${TEST_NAME}_copy -E copy_if_different
    #         $<TARGET_FILE:common>
    #         $<TARGET_FILE_DIR:${TEST_NAME}>
    # )
    copy_dll(${TEST_NAME} common)

    # register test in CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
