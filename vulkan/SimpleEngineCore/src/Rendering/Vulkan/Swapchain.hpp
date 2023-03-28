#pragma once
#include "WindowSurface.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>
#include "VulkanUtility.hpp"
#include <algorithm>
#include <limits>

namespace SimpleEngine
{
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };


    SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR& surface) {
        SwapChainSupportDetails details;

        details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
        details.formats      = device.getSurfaceFormatsKHR(surface);
        details.presentModes = device.getSurfacePresentModesKHR(surface);

        return details;
    }


    class SwapChain
    {
    public:
        static vk::SwapchainKHR make(VulkanWindow* window, vk::SurfaceKHR& surface, vk::PhysicalDevice& physicalDevice, vk::Device& device,
                  std::vector<vk::Image>& swapChainImages, vk::Format& swapChainImageFormat, vk::Extent2D& swapChainExtent) {
            
            vk::SwapchainKHR swapChain;
            
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

            vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
            vk::PresentModeKHR presentMode     = chooseSwapPresentMode(swapChainSupport.presentModes);
            vk::Extent2D extent                = chooseSwapExtent(window, swapChainSupport.capabilities);

            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

            vk::SwapchainCreateInfoKHR createInfo {
                vk::SwapchainCreateFlagsKHR(),
                surface,
                imageCount,
                surfaceFormat.format,
                surfaceFormat.colorSpace,
                extent,
                1,
                vk::ImageUsageFlagBits::eColorAttachment,
            };

            QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
            uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

            if (indices.graphicsFamily != indices.presentFamily) {
                createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            } else {
                createInfo.imageSharingMode = vk::SharingMode::eExclusive;
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;

            vk::resultCheck(device.createSwapchainKHR(&createInfo, nullptr, &swapChain), "failed to create swap chain!");

            swapChainImages =  device.getSwapchainImagesKHR(swapChain);
            swapChainImageFormat = surfaceFormat.format;
            swapChainExtent = extent;

            return swapChain;
        }
    
    private:

        static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
            for (auto&& availableFormat : availableFormats) {
                if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT) {
                    return availableFormat;
                }
            }
            return availableFormats[0];
        }

        static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
            for (auto&& availablePresentMode : availablePresentModes) {
                if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                    return availablePresentMode;
                }
            }
            return vk::PresentModeKHR::eFifo;
        }

        static vk::Extent2D chooseSwapExtent(VulkanWindow* window, const vk::SurfaceCapabilitiesKHR& capabilities) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            } else {
                int width, height;
                window->getFramebufferSize(&width, &height);

                vk::Extent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
                };

                actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                return actualExtent;
            }
        }

    
    };
}