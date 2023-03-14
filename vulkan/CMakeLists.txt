cmake_minimum_required(VERSION 3.19)

project (Triangles VERSION 1.0.1)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build>)
# set(EXECSOURCES "${CMAKE_SOURCE_DIR}/source" )

# set(CPP_SOURCES
#     "main.cpp"
#     "src/vulkan.cpp"
# )

aux_source_directory(./src CPP_SOURCES)


set( EXEC_NAME "vulkan" )
# add_executable( ${EXEC_NAME} )
find_package(Vulkan REQUIRED)

add_executable (${EXEC_NAME} main.cpp ${CPP_SOURCES})
target_link_libraries(${EXEC_NAME} glm)
target_link_libraries(${EXEC_NAME} glfw)
target_link_libraries(${EXEC_NAME} Vulkan::Vulkan)
target_include_directories(${EXEC_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(${EXEC_NAME} PUBLIC ./include)


