cmake_minimum_required(VERSION 3.5)

project(Triangles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE RELEASE)


add_subdirectory(3rd-party/glfw   EXCLUDE_FROM_ALL)
add_subdirectory(3rd-party/glm    EXCLUDE_FROM_ALL)
add_subdirectory(3rd-party/spdlog EXCLUDE_FROM_ALL)


add_subdirectory(test)
add_subdirectory(vulkan)