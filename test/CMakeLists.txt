cmake_minimum_required(VERSION 3.15)

# Find test framework
find_package(unity REQUIRED)

# Collect test source files
file(GLOB TEST_SOURCES
    src/*.c
)

foreach(TEST_FILE ${TEST_SOURCES})
    # get test name without path and extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # create executable
    add_executable(${TEST_NAME} ${TEST_FILE})

    # link unity and project library
    target_link_libraries(${TEST_NAME}
        PUBLIC
            unity::unity
            ${PROJECT_NAME}
    )

    # register test in CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
