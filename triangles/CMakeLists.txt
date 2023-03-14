project(triangle)

#aux_source_directory(./src CPP_SOURCES)
set(CPP_SOURCES
        "main.cpp"
        "src/intersect.cpp"
        "src/line.cpp"
        "src/octree.cpp"
        "src/plane.cpp"
        "src/segment.cpp"
        "src/triangle.cpp"
        "src/vector.cpp"
        "src/vulkan.cpp"
)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} ${CPP_SOURCES} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ./include)


target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${VULKAN_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
