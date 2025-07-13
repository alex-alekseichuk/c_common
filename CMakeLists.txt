cmake_minimum_required(VERSION 3.15)
project(common C)

SET(SRC
    src/main.c
)

add_library(${PROJECT_NAME} SHARED ${SRC})

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define COMMON_BUILD_DLL when building the DLL
target_compile_definitions(${PROJECT_NAME} PRIVATE COMMON_BUILD_DLL)

# Optional: Install target (headers + library)
install(TARGETS common
    ARCHIVE DESTINATION lib  # For STATIC lib
    LIBRARY DESTINATION lib  # For SHARED lib
)
install(DIRECTORY include/ DESTINATION include)

