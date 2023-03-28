#pragma once
#include <vulkan/vulkan.hpp>
#include "ValidationLayer.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SimpleEngine 
{
    class Instance : public vk::Instance
    {
    public:
        Instance()
        {
            if (ValidationLayer::enable && !ValidationLayer::checkSupport()) {
                throw std::runtime_error("validation layers requested, but not available!");
            }

            vk::ApplicationInfo appInfo {
                    "3D Triangle Application",
                    VK_API_VERSION_1_0,
                    "No Engine",
                    VK_API_VERSION_1_0,
                    VK_API_VERSION_1_0
            };

            auto&& extensions = getRequiredExtensions();
            vk::InstanceCreateInfo createInfo {
                    vk::InstanceCreateFlags(),
                    &appInfo,
                    ValidationLayer::validationLayers,
                    extensions
            };

            vk::resultCheck(vk::createInstance(&createInfo, nullptr, this), "failed to create instance!");
        }

        ~Instance() {
            destroy();
        }

    private:

        std::vector<const char*> getRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if (ValidationLayer::enable) {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
            return extensions;
        }
    };
}

