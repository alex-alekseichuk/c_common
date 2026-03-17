cmake_minimum_required(VERSION 3.15)
project(common C)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Option to enable/disable tests
option(BUILD_TESTS "Build tests" ON)

# to avoid passing -DCMAKE_EXPORT_COMPILE_COMMANDS=1 all the time
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC
    src/allocator.c
    src/malloc.c
)

add_library(${PROJECT_NAME} SHARED ${SRC})

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define COMMON_BUILD_DLL when building the DLL
target_compile_definitions(${PROJECT_NAME} PRIVATE COMMON_BUILD_DLL)

# Build tests if enabled
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Optional: Install target (headers + library)
install(TARGETS common
    ARCHIVE DESTINATION lib  # For STATIC lib
    LIBRARY DESTINATION lib  # For SHARED lib
)
install(DIRECTORY include/ DESTINATION include)


# Auto-symlink to project root
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()
