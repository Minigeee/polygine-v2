cmake_minimum_required(VERSION 3.14)
project(polygine VERSION 0.1.0 LANGUAGES C CXX)

# Set C++20 as the standard for the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g3)
    add_compile_options(-O0)
endif()

# Coord system
add_compile_definitions(GLM_FORCE_LEFT_HANDED)
add_compile_definitions(GLM_FORCE_DEPTH_ZERO_TO_ONE)

# Build the engine library first
add_subdirectory(src)
add_subdirectory(extlibs)

# Create the development executable
add_executable(polygine_dev 
    examples/main.cpp    # Your development/testing code
)

# Link the executable with the engine library
target_link_libraries(polygine_dev 
    PRIVATE 
        polygine
)

# Optional: Set the executable output directory
set_target_properties(polygine_dev
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
copy_required_dlls(polygine_dev)

# Copy SDL3 DLL to the output directory
if(WIN32)
    add_custom_command(TARGET polygine_dev POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:polygine_dev>
    )
endif()
