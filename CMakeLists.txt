cmake_minimum_required(VERSION 3.15)
project(common C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# to avoid passing -DCMAKE_EXPORT_COMPILE_COMMANDS=1 all the time
# DCMAKE_EXPORT_COMPILE_COMMANDS creates compile_commands.json for vscode/cursor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC
    src/allocator.c
    src/sys_alloc.c
    src/arena_alloc.c
    src/array.c
    src/list.c
    src/rb_tree.c
    src/str.c
)

add_library(${PROJECT_NAME} SHARED ${SRC})

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion>
    $<$<C_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Define COMMON_BUILD_DLL when building the DLL
target_compile_definitions(${PROJECT_NAME} PRIVATE COMMON_BUILD_DLL PUBLIC COMMON_SHARED)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Build tests if enabled
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
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
