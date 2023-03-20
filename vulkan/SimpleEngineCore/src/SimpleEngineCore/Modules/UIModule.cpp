#include "UIModule.hpp"

#include <imgui/imgui.h>
namespace SimpleEngine {
    void UIModule::on_window_create(GLFWwindow* pWindow, ImGui_ImplVulkan_InitInfo* info, VkRenderPass pass)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplVulkan_Init(info, pass);
        ImGui_ImplGlfw_InitForVulkan(pWindow, true);
    }

    void UIModule::on_window_close()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UIModule::on_ui_draw_begin()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void UIModule::on_ui_draw_end(VkCommandBuffer pBuffer, VkPipeline pPipeline)
    {
        ImGui::ShowDemoWindow();

        ImGui::Render();
        // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), pBuffer, pPipeline);
        // if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        // {
        //     GLFWwindow* backup_current_context = glfwGetCurrentContext();
        //     ImGui::UpdatePlatformWindows();
        //     ImGui::RenderPlatformWindowsDefault();
        //     glfwMakeContextCurrent(backup_current_context);
        // }
        // ImGui::EndFrame();
    }
}