cmake_minimum_required(VERSION 3.5)

project(Triangles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(test)
add_subdirectory(vulkan)