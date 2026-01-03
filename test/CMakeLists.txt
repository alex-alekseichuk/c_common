cmake_minimum_required(VERSION 3.15)

# Find test framework
find_package(unity REQUIRED)

# Collect test source files
file(GLOB TEST_SOURCES
    src/*.c
)

# Create test executable
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

# Link with Unity and our library
target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        unity::unity
        common
)

# Include directories
target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Enable testing
enable_testing()

# Add test command
add_test(NAME ${PROJECT_NAME}_unit_tests COMMAND ${PROJECT_NAME}_tests)
