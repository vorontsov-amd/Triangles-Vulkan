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
//            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
//            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//            createInfo.pfnUserCallback = ValidationLayer::debugCallback;
        }

        static void setup(vk::Instance& instance, vk::DebugUtilsMessengerEXT& debugMessenger) {
            if (!ValidationLayer::enable) return;
            vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = DebugMessenger::populateDebugMessengerCreateInfo();
            vk::DispatchLoaderDynamic loaderDynamic {instance, vkGetInstanceProcAddr};
            vk::resultCheck(instance.createDebugUtilsMessengerEXT(&debugCreateInfo, nullptr, &debugMessenger, loaderDynamic), "failed to create debug messenger!");
        }

        static void destroy(vk::Instance& instance, vk::DebugUtilsMessengerEXT& debugMessenger) {
            if (!ValidationLayer::enable) return;
            vk::DispatchLoaderDynamic loaderDynamic {instance, vkGetInstanceProcAddr};
            instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, loaderDynamic);
        }

    };
}