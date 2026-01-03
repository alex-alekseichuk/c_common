cmake_minimum_required(VERSION 3.15)
project(common C)

# Option to enable/disable tests
option(BUILD_TESTS "Build tests" ON)

file(GLOB SRC
    src/allocator.c
)

add_library(${PROJECT_NAME} SHARED ${SRC})

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define COMMON_BUILD_DLL when building the DLL
target_compile_definitions(${PROJECT_NAME} PRIVATE COMMON_BUILD_DLL)

# Build tests if enabled
if(BUILD_TESTS)
    add_subdirectory(test)
endif()

# Optional: Install target (headers + library)
install(TARGETS common
    ARCHIVE DESTINATION lib  # For STATIC lib
    LIBRARY DESTINATION lib  # For SHARED lib
)
install(DIRECTORY include/ DESTINATION include)

