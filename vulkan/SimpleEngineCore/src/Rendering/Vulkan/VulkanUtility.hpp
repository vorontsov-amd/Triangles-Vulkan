#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

namespace SimpleEngine
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };


    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR& surface) {
        QueueFamilyIndices indices;

        auto&& queueFamilies = device.getQueueFamilyProperties();

        for (auto i = 0U; auto&& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphicsFamily = i;
            }

            vk::Bool32 presentSupport = false;
            vk::resultCheck(device.getSurfaceSupportKHR(i, surface, &presentSupport), "failed to get surface support");

            if (presentSupport) indices.presentFamily = i;
            if (indices.isComplete()) break;
            i++;
        }
        return indices;
    }


}