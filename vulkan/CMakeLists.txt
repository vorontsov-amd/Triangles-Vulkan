# # project (triangles)

# # aux_source_directory(./src CPP_SOURCES)

# # find_package(Vulkan REQUIRED)

# # add_executable (${PROJECT_NAME} main.cpp ${CPP_SOURCES})
# # target_link_libraries(${PROJECT_NAME} glm)
# # target_link_libraries(${PROJECT_NAME} glfw)
# # target_link_libraries(${PROJECT_NAME} spdlog)
# # target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
# # target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
# # target_include_directories(${PROJECT_NAME} PUBLIC ./include)




# cmake_minimum_required(VERSION 3.12)

# set(EDITOR_PROJECT_NAME SimpleEngineEditor)

# add_executable(${EDITOR_PROJECT_NAME}
# 	main.cpp
# )

# find_package(Vulkan REQUIRED)
# target_include_directories(${EDITOR_PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
# target_link_libraries(${EDITOR_PROJECT_NAME} SimpleEngineCore ImGui glm Vulkan::Vulkan)
# target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

# set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)


# set(VERT_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv")
# set(FRAG_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv")
# set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/sourcePath.h")
# configure_file(./include/sourcePath.h.in ${OUTPUT_PATH} @ONLY)

# # install(TARGETS ${PROJECT_NAME}
# #         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# #         COMPONENT ${PROJECT_NAME})

set(PROJECT_NAME SimpleEngine)
project(${PROJECT_NAME})

# aux_source_directory(./GeomLib GEOM_LIB)

add_subdirectory(SimpleEngineCore)
add_subdirectory(SimpleEngineEditor)

# set(VERT_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv")
# set(FRAG_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv")
# set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/sourcePath.h")
# configure_file(./include/sourcePath.h.in ${OUTPUT_PATH} @ONLY)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleEngineEditor)

