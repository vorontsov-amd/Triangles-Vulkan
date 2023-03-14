cmake_minimum_required(VERSION 3.19)

project (Triangles VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build>)
set(EXECSOURCES "${CMAKE_SOURCE_DIR}/source" )

set(SOURCES
    "vulkan.cpp"
)

set( EXEC_NAME "triangles" )
# add_executable( ${EXEC_NAME} )


find_package(Threads)


add_subdirectory(3rdparty/glfw)
include_directories(3rdparty/glfw/include)

add_subdirectory(3rdparty/glm)
include_directories(3rdparty/glm)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(vulkan/shaders)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/vulkan/shaders)

add_executable (${EXEC_NAME})
target_link_libraries(${EXEC_NAME} glm)
# target_link_libraries(${EXEC_NAME} PRIVATE glfw Vulkan::Vulkan glm)


add_dependencies(${EXEC_NAME} Shaders)

target_include_directories( ${EXEC_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)

target_include_directories(${EXEC_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} Vulkan::Vulkan)


target_link_libraries(${EXEC_NAME} glfw)
