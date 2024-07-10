cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # we want to use the static runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

project(Avalon)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)          # window opener
add_subdirectory(dependencies/glad)           # opengl loader
add_subdirectory(dependencies/glm)           # math
#[[add_subdirectory(dependencies/stb_truetype)  # loading ttf files
add_subdirectory(dependencies/imgui-docking) # ui]]

# Add an interface library for stb_image
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb)

# Define MY_SOURCES to be a list of all the source files for my game
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME})

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Use relative path for resources
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC RESOURCES_PATH="./resources/")

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MY_SOURCES})

if(MSVC) # If using the VS compiler...
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)

    #remove console
    #set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
endif()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm glfw glad stb_image#[[ stb_truetype imgui]])
