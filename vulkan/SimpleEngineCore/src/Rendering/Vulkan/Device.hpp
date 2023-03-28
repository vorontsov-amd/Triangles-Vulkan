#pragma once
#include "ValidationLayer.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanUtility.hpp"
#include <set>
#include <string>
#include <array>
#include "Swapchain.hpp"

namespace SimpleEngine
{
    class PhysicalDevice
    {
    public:

        static constexpr std::array<const char*, 1> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        static vk::PhysicalDevice make(vk::Instance& instance, vk::SurfaceKHR& surface) {
            vk::PhysicalDevice physicalDevice = nullptr;

            auto&& devices = instance.enumeratePhysicalDevices();
            if (devices.empty()) {
                throw std::runtime_error("failed to find GPUs with Vulkan support!");
            }

            for (auto&& device : devices) {
                if (isDeviceSuitable(device, surface)) {
                    physicalDevice = device;
                    break;
                }
            }

            if (!physicalDevice) {
                throw std::runtime_error("failed to find a suitable GPU!");
            }

            return physicalDevice;
        }

    private:

        static bool isDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR& surface) {
            QueueFamilyIndices indices = findQueueFamilies(device, surface);

            bool extensionsSupported = checkDeviceExtensionSupport(device);

            bool swapChainAdequate = false;
            if (extensionsSupported) {
                SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
                swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
            }

            return indices.isComplete() && extensionsSupported && swapChainAdequate;
        }

        static bool checkDeviceExtensionSupport(vk::PhysicalDevice device) {
            auto&& availableExtensions = device.enumerateDeviceExtensionProperties();
            std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
            for (auto&& extension : availableExtensions) {
                requiredExtensions.erase(extension.extensionName);
            }
            return requiredExtensions.empty();
        }
    };





    class LogicalDevice
    {
    public:
        static vk::Device make(vk::Instance& instance, vk::SurfaceKHR& surface, 
                               vk::PhysicalDevice& physicalDevice, vk::Queue& graphicsQueue, vk::Queue& presentQueue) {
            
            vk::Device device;
            
            QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

            std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
            std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

            float queuePriority = 1.0f;
            for (auto&& queueFamily : uniqueQueueFamilies) {
                vk::DeviceQueueCreateInfo queueCreateInfo {
                    vk::DeviceQueueCreateFlags(),
                    queueFamily,
                    1,
                    &queuePriority
                };
                queueCreateInfos.push_back(queueCreateInfo);
            }

            vk::PhysicalDeviceFeatures deviceFeatures{};
            vk::DeviceCreateInfo createInfo {
                vk::DeviceCreateFlags(),
                queueCreateInfos,
                ValidationLayer::validationLayers,
                PhysicalDevice::deviceExtensions,
                &deviceFeatures
            };

            vk::resultCheck(physicalDevice.createDevice(&createInfo, nullptr, &device), "failed to create logical device!");
            device.getQueue(indices.graphicsFamily.value(), 0, &graphicsQueue);
            device.getQueue(indices.presentFamily.value(), 0, &presentQueue);

            return device;
        }
    };
}