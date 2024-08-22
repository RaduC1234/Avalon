cmake_minimum_required(VERSION 3.16)
project(Avalon LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)  # Include Avalon/src

# Add dependencies
add_subdirectory(dependencies/spdlog)   # Logging
add_subdirectory(dependencies/glfw)     # OpenGL window and context
add_subdirectory(dependencies/glad)     # OpenGL loader
add_subdirectory(dependencies/glm)      # OpenGL Math
add_subdirectory(dependencies/json)     # JSON handling
add_subdirectory(dependencies/freetype) # Text rendering

# Add an interface library for stb_image - it doesn't have its own CMakeLists.txt
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb)

# Add an interface library for stb_image - it doesn't have its own CMakeLists.txt
add_library(imgui INTERFACE)
target_include_directories(imgui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui)
target_include_directories(imgui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/backends)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the libraries to the executable
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        spdlog::spdlog          # Logging
        glfw                    # OpenGL window and context
        glad                    # OpenGL loader
        glm                     # OpenGL Math
        stb_image               # Image loading
        nlohmann_json::nlohmann_json  # JSON handling
        freetype                # Text rendering
        imgui
)

# Set compile options for different build configurations
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>: -DAVALON_DEBUG>
        $<$<CONFIG:Release>: -DAVALON_RELEASE>
)

# Set symbols and optimization for different build configurations
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>: -g>
        $<$<CONFIG:Release>: -O3>
)
