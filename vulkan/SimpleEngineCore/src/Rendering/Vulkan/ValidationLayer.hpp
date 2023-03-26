#pragma once
#include <vulkan/vulkan.hpp>
#include <string>
#include <array>
#include <algorithm>
#include <iostream>
#include <cstring>

namespace SimpleEngine {
    struct ValidationLayer
    {

    #ifdef NDEBUG
        static constexpr const bool enable = false;
        static constexpr const std::array<const char*, 1> validationLayers = {};
    #else
        static constexpr const bool enable = true;
        static constexpr const std::array<const char*, 1> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    #endif

        static bool checkSupport() {
            auto&& availableLayers = vk::enumerateInstanceLayerProperties();

            return std::find_if(validationLayers.begin(), validationLayers.end(), [&availableLayers](auto&& layerName) {
                return std::find_if(availableLayers.begin(), availableLayers.end(), [&layerName](auto&& layer) {
                    return std::strcmp(layer.layerName, layerName) == 0;
                }) != availableLayers.end();
            }) != validationLayers.end();

        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
            return VK_FALSE;
        }
    };
} // namespace SimpleEngine


