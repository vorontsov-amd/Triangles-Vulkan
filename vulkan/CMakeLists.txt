project (triangles)

aux_source_directory(./src CPP_SOURCES)

find_package(Vulkan REQUIRED)

add_executable (${PROJECT_NAME} main.cpp ${CPP_SOURCES})
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} spdlog)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ./include)


set(VERT_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv")
set(FRAG_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv")
set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/sourcePath.h")
configure_file(./include/sourcePath.h.in ${OUTPUT_PATH} @ONLY)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})


