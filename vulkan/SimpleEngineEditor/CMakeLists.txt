cmake_minimum_required(VERSION 3.12)

set(EDITOR_PROJECT_NAME SimpleEngineEditor)

add_executable(${EDITOR_PROJECT_NAME}
	src/VkEngineEditor.cpp
)

find_package(Vulkan REQUIRED)
target_include_directories(${EDITOR_PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${EDITOR_PROJECT_NAME} SimpleEngineCore ImGui glm Vulkan::Vulkan)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)



set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)