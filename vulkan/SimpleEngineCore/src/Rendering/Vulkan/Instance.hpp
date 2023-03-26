#pragma once

#include "ValidationLayer.hpp"
#include <vulkan/vulkan_enums.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "DebugMessenger.hpp"

namespace SimpleEngine {

    class Instance
    {
    private:
//        vk::Instance instance;
//        vk::DebugUtilsMessengerEXT messenger;
//    public:
//        Instance() {
//            if (ValidationLayer::enable && !ValidationLayer::checkSupport()) {
//                throw std::runtime_error("validation layers requested, but not available!");
//            }
//
//            vk::ApplicationInfo appInfo {
//                "3D Triangle Application",
//                VK_API_VERSION_1_0,
//                "No Engine",
//                VK_API_VERSION_1_0,
//                VK_API_VERSION_1_0
//            };
//
//            auto&& extensions = getRequiredExtensions();
//            vk::InstanceCreateInfo createInfo {
//                vk::InstanceCreateFlags(),
//                &appInfo,
//                ValidationLayer::validationLayers,
//                extensions
//            };
//
//            vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = DebugMessenger::populateDebugMessengerCreateInfo();
//            if (ValidationLayer::enable) {
//                createInfo.pNext = &debugCreateInfo;
//            }
//            vk::resultCheck(vk::createInstance(&createInfo, nullptr, &instance), "failed to create instance!");
//            vk::resultCheck(instance.createDebugUtilsMessengerEXT(&debugCreateInfo, nullptr, &messenger), "failed to create debug messenger!");
//        }

//        void createSurface(vk::SurfaceKHR& surface, GLFWwindow* window)
//        {
//            VkSurfaceKHR surfaceCore {};
//            if (glfwCreateWindowSurface(instance, window, nullptr, &surfaceCore) != VK_SUCCESS) {
//                throw std::runtime_error("failed to create window surface!");
//            }
//
//            surface = surfaceCore;
//        }

    public:

        //! TODO glfwpp-wrapper
        static std::vector<const char*> getRequiredExtensions() {
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

