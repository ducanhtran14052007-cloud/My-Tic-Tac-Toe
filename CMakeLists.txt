cmake_minimum_required(VERSION 3.10)

# 1. Project name and C++ standard configuration
project(TicTacToe-LV2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Recursively find all source files in src/ and its subdirectories
# This will include interaction/, logic/, renderer/, utils/, etc.
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 3. Find SDL packages
# Note: You can comment out one of these depending on which version you actually use
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# find_package(SDL3 REQUIRED) # Uncomment this if you switch to SDL3

# 4. Define the executable target
add_executable(game ${SOURCES})

# 5. Add include directories
# Using target_include_directories is the modern CMake approach
target_include_directories(game PRIVATE 
    src 
    # src/interaction 
    # src/logic 
    # src/renderer 
    # src/utils
    # This allows you to use #include "logger.h" instead of "src/utils/logger.h"
    ${SDL2_INCLUDE_DIRS} # Include path for SDL2
    # ${SDL3_INCLUDE_DIRS} # Include path for SDL3
)

# 6. Link libraries
# This connects the SDL binary files to your project
target_link_libraries(game PRIVATE 
    SDL2::SDL2main
    SDL2::SDL2         # Link SDL2 (standard way)
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
    # SDL3::SDL3       # Link SDL3 (standard way)
)

# 7. Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# 8. Compiler options for warnings and optimization
if(MSVC)
    target_compile_options(game PRIVATE /W4 /O2)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -O2)
endif()

# Copy assets/ to build/
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:game>/assets
)