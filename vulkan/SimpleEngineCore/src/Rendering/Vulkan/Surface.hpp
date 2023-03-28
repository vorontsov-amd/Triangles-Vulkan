#pragma once
#include <vulkan/vulkan.hpp>
#include "ValidationLayer.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SimpleEngine
{
    class Surface : public vk::SurfaceKHR
    {
    public:
        Surface(vk::Instance& instance, GLFWwindow* window)
        {
            VkSurfaceKHR surfaceCore {};
            if (glfwCreateWindowSurface(instance, window, nullptr, &surfaceCore) != VK_SUCCESS) {
                throw std::runtime_error("failed to create window surface!");
            }
            static_cast<vk::SurfaceKHR>(*this) = surfaceCore;
        }

    };

}