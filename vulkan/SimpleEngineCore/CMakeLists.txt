cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME SimpleEngineCore)

set(ENGINE_PUBLIC_INCLUDES
	includes/SimpleEngineCore/Application.hpp
	includes/SimpleEngineCore/Log.hpp
	includes/SimpleEngineCore/Event.hpp
	includes/SimpleEngineCore/Camera.hpp
	includes/SimpleEngineCore/Keys.hpp
	includes/SimpleEngineCore/Input.hpp
	src/SimpleEngineCore/Rendering/Vulkan/VulkanRenderer.hpp
	src/SimpleEngineCore/Rendering/Vulkan/VertexArray.hpp

)

set(ENGINE_PRIVATE_INCLUDES
	src/SimpleEngineCore/Window.hpp
	src/SimpleEngineCore/Modules/UIModule.hpp
	src/SimpleEngineCore/sourcePath.h
	src/SimpleEngineCore/sourcePath.h.in
)

set(ENGINE_PRIVATE_SOURCES
	src/SimpleEngineCore/Application.cpp
	src/SimpleEngineCore/Window.cpp
	src/SimpleEngineCore/Input.cpp
	src/SimpleEngineCore/Modules/UIModule.cpp
	src/SimpleEngineCore/Camera.cpp
	src/SimpleEngineCore/Rendering/Vulkan/VulkanRenderer.cpp
	src/SimpleEngineCore/Rendering/Vulkan/vulkan.cpp
)

set(ENGINE_ALL_SOURCES
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)

# set(VERT_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vert.spv")
# set(FRAG_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/frag.spv")
# set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleEngineCore/sourcePath.h")
# configure_file(./src/SimpleEngineCore/sourcePath.h.in ${OUTPUT_PATH} @ONLY)


add_library(${ENGINE_PROJECT_NAME} STATIC
	${ENGINE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${ENGINE_ALL_SOURCES}
)

find_package(Vulkan REQUIRED)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE Vulkan::Vulkan)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../../3rd-party/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../../3rd-party/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

# add_subdirectory(../../3rd-party/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
# target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../../3rd-party/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)

set(IMGUI_INCLUDES
	../../3rd-party/imgui/imgui.h
	../../3rd-party/imgui/backends/imgui_impl_glfw.h
	../../3rd-party/imgui/backends/imgui_impl_vulkan.h
)

set(IMGUI_SOURCES
	../../3rd-party/imgui/imgui.cpp
	../../3rd-party/imgui/backends/imgui_impl_glfw.cpp
	../../3rd-party/imgui/backends/imgui_impl_vulkan.cpp
	../../3rd-party/imgui/imgui_demo.cpp
	../../3rd-party/imgui/imgui_tables.cpp
	../../3rd-party/imgui/imgui_widgets.cpp
	../../3rd-party/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../../3rd-party)
target_include_directories(ImGui PUBLIC ../../3rd-party/imgui)
target_link_libraries(ImGui PRIVATE glfw)

target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)