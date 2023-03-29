#pragma once
#include "Window.hpp" 
#include "ValidationLayer.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>

namespace SimpleEngine
{
    class VulkanWindow : public Window
    {
    public:
        VkResult createWindowSurface(VkInstance instance, VkSurfaceKHR* pSurface) {
            return glfwCreateWindowSurface(instance, m_pWindow, nullptr, pSurface);
        }

        void getFramebufferSize(int* width, int* height) {
            return glfwGetFramebufferSize(m_pWindow, width, height);
        }

        static VulkanWindow* make(const std::unique_ptr<Window>& pWindow) {
            return static_cast<VulkanWindow*>(&*pWindow);
        }
    };

    class Surface
    {
    public:
        static vk::SurfaceKHR make(VkInstance instance, VulkanWindow* window) {
            VkSurfaceKHR surfaceCore {};
            if (window->createWindowSurface(instance, &surfaceCore) != VK_SUCCESS) {
                throw std::runtime_error("failed to create window surface!");
            }
            return static_cast<vk::SurfaceKHR>(surfaceCore);
        }
    };
}