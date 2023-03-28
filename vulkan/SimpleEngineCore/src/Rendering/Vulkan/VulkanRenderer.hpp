#pragma once

#include <memory>
#include "Window.hpp"
#include <vulkan/vulkan.hpp>
#include "sourcePath.h"
#include <glm/glm.hpp>
// #include "SimpleEngineCore/Rendering/Vulkan/VertexArray.hpp"
#include "Instance.hpp"
#include "ValidationLayer.hpp"
#include "Rendering/Vulkan/VertexArray.hpp"
#include "WindowSurface.hpp"
#include "Device.hpp"

#include <iostream>

namespace SimpleEngine {
    struct SwapChainSupportDetails;
    struct QueueFamilyIndices;


    class VulkanRenderer {
    public:        
        explicit VulkanRenderer(const std::unique_ptr<Window>& pWindow);
        ~VulkanRenderer();

        void drawFrame(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera_pos);
        static void setVertexArray(VertexArray& array); 
        static void setIndexArray(IndexArray& array); 

    private:
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createDescriptorSetLayout();
        void createGraphicsPipeline();
        void createCommandPool();
        void createDepthResources();
        void createFramebuffers();
        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createCommandBuffers();
        void createSyncObjects();


        vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const;
        vk::Format findDepthFormat();
        static bool hasStencilComponent(vk::Format format);
        vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const;
        void createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);
        void cleanupSwapChain();
        void recreateSwapChain();
        void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
        void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
        void recordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex);
        void updateUniformBuffer(uint32_t currentImage, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera_pos);
        vk::ShaderModule createShaderModule(const std::vector<char>& code) const;
        static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;
        SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device) const;
        bool isDeviceSuitable(vk::PhysicalDevice device);
        static bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
        static std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();

    private:
        VulkanWindow* window;
        Instance instance;
        vk::SurfaceKHR surface;
        vk::DebugUtilsMessengerEXT debugMessenger;
        
        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        EventDispatcher m_event_dispatcher;
        bool framebufferResized = false;

        vk::SwapchainKHR swapChain;
        std::vector<vk::Image> swapChainImages;
        vk::Format swapChainImageFormat;
        vk::Extent2D swapChainExtent;
        std::vector<vk::ImageView> swapChainImageViews;
        std::vector<vk::Framebuffer> swapChainFramebuffers;

        vk::RenderPass renderPass;
        vk::DescriptorSetLayout descriptorSetLayout;
        vk::PipelineLayout pipelineLayout;
        vk::Pipeline graphicsPipeline;

        vk::CommandPool commandPool;

        vk::Buffer vertexBuffer;
        vk::DeviceMemory vertexBufferMemory;
        vk::Buffer indexBuffer;
        vk::DeviceMemory indexBufferMemory;

        std::vector<vk::Buffer> uniformBuffers;
        std::vector<vk::DeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        vk::DescriptorPool descriptorPool;
        std::vector<vk::DescriptorSet> descriptorSets;

        std::vector<vk::CommandBuffer> commandBuffers;

        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;
        uint32_t currentFrame = 0;

        vk::Image depthImage;
        vk::DeviceMemory depthImageMemory;
        vk::ImageView depthImageView;
    };
}