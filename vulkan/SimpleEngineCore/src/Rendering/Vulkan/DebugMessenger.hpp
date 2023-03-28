#pragma once
#include "ValidationLayer.hpp"
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

namespace SimpleEngine
{
    class DebugMessenger
    {
    public:
        static vk::DebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo() {
            vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo {
                vk::DebugUtilsMessengerCreateFlagsEXT(),
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                ValidationLayer::debugCallback
            };
            return debugCreateInfo;
        }

        static vk::DebugUtilsMessengerEXT make(vk::Instance& instance) {
            vk::DebugUtilsMessengerEXT messenger;
            setup(instance, messenger);
            return messenger;
        }

        static void destroy(vk::Instance& instance, vk::DebugUtilsMessengerEXT& debugMessenger) {
            if (!ValidationLayer::enable) return;
            vk::DispatchLoaderDynamic loaderDynamic {instance, vkGetInstanceProcAddr};
            instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, loaderDynamic);
        }

    private:

        static void setup(vk::Instance& instance, vk::DebugUtilsMessengerEXT& debugMessenger) {
            if (!ValidationLayer::enable) return;
            vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = DebugMessenger::populateDebugMessengerCreateInfo();
            vk::DispatchLoaderDynamic loaderDynamic {instance, vkGetInstanceProcAddr};
            vk::resultCheck(instance.createDebugUtilsMessengerEXT(&debugCreateInfo, nullptr, &debugMessenger, loaderDynamic), "failed to create debug messenger!");
        }

    };
}