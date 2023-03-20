#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/backends/imgui_impl_glfw.h>


namespace SimpleEngine {

    class UIModule
    {
    private:
        // VkCommandBuffer pBuffer;
        // VkPipeline pPipeline;
    public:
        static void on_window_create(GLFWwindow* pWindow, ImGui_ImplVulkan_InitInfo* info, VkRenderPass pass);
        static void on_window_close();
        static void update_command_buffer(VkCommandBuffer* buffer, VkPipeline* pipeline); 
        static void on_ui_draw_begin();
        //static void on_ui_draw_end();

        static void on_ui_draw_end(VkCommandBuffer pBuffer, VkPipeline pPipeline);


    };

}