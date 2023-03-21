#pragma once

#include <memory>
#include "SimpleEngineCore/Window.hpp"
#include <vulkan/vulkan.hpp>
#include "SimpleEngineCore/sourcePath.h"
#include <glm/glm.hpp>
// #include "SimpleEngineCore/Rendering/Vulkan/VertexArray.hpp"

#include "SimpleEngineCore/Rendering/Vulkan/VertexArray.hpp"

#include <iostream>

namespace SimpleEngine {
    struct SwapChainSupportDetails;
    struct QueueFamilyIndices;


    class VulkanRenderer {
    public:        
        VulkanRenderer(const std::unique_ptr<Window>& pWindow);
        
        void init(GLFWwindow* pWindow) { window = pWindow; }
        void drawFrame(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera_pos);
        static void setVertexArray(VertexArray& array); 
        static void setIndexArray(IndexArray& array); 

        // static void draw(const VertexArray& vertex_array);
        // static void set_clear_color(const float r, const float g, const float b, const float a);
        // static void clear();
        // static void set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);
        // static void enable_depth_test();
        // static void disable_depth_test();

        // static const char* get_vendor_str();
        // static const char* get_renderer_str();
        // static const char* get_version_str();
    private:
        void createInstance();
        void setupDebugMessenger();
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
        void initImgui();


        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        bool hasStencilComponent(VkFormat format);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void cleanupSwapChain();
        void cleanup();
        void recreateSwapChain();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void updateUniformBuffer(uint32_t currentImage, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera_pos);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        VkCommandBuffer beginSingleTimeCommands();

    public:
        GLFWwindow* window;

        EventDispatcher m_event_dispatcher;
        bool framebufferResized = false;
        int framebufferWidth = 0, framebufferHeight = 0;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        int min_image_count;

        VkRenderPass imguiPass;
        VkRenderPass renderPass;
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        VkCommandPool commandPool;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        VkDescriptorPool imguiPool;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t currentFrame = 0;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
    };
}