#include "VulkanRenderer.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "ValidationLayer.hpp"
#include "Log.hpp"
#include "DebugMessenger.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdint>
#include <optional>
#include <set>
#include "VulkanUtility.hpp"

namespace SimpleEngine {

    const int MAX_FRAMES_IN_FLIGHT = 2;

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
        alignas(16) glm::vec3 view_pos;
    };


    static std::vector<Vertex> vertices;
    static std::vector<uint16_t> indices;

    void VulkanRenderer::setVertexArray(VertexArray& array) { 
        std::copy(array.begin(), array.end(), std::back_inserter(vertices));
    }
    void VulkanRenderer::setIndexArray(IndexArray& array) {
        std::copy(array.begin(), array.end(), std::back_inserter(indices)); 
    }


    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }


    VulkanRenderer::VulkanRenderer(const std::unique_ptr<Window>& pWindow) : 
        window          {VulkanWindow::  make(pWindow)}, 
        surface         {Surface::       make(instance, window)},
        debugMessenger  {DebugMessenger::make(instance)},
        physicalDevice  {PhysicalDevice::make(instance, surface)},
        device          {LogicalDevice:: make(instance, surface, physicalDevice, graphicsQueue, presentQueue)},
        swapChain       {SwapChain::     make(window, surface, physicalDevice, device, swapChainImages, swapChainImageFormat, swapChainExtent)}
    
    {
        m_event_dispatcher.add_event_listener<EventFramebufferResize>(
            [&](EventFramebufferResize& event)
            {
                LOG_INFO("Framebuffer was resized");
                framebufferResized = true;
            }
        );

        pWindow->set_event_callback(
            [&](BaseEvent& event)
            {
                m_event_dispatcher.dispatch(event);
            }
        );

        createImageViews();
        createRenderPass();
        createDescriptorSetLayout();
        createGraphicsPipeline();
        createCommandPool();
        createDepthResources();
        createFramebuffers();
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        createSyncObjects();
    }

    vk::Format VulkanRenderer::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const {
        for (auto&& format : candidates) {
            vk::FormatProperties props;
            physicalDevice.getFormatProperties(format, &props);

            if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        throw std::runtime_error("failed to find supported format!");
    }

    vk::Format VulkanRenderer::findDepthFormat() {
        return findSupportedFormat(
            {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );
    }

    bool VulkanRenderer::hasStencilComponent(vk::Format format) {
        return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
    }


    vk::ImageView VulkanRenderer::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const {
        vk::ImageViewCreateInfo viewInfo {
            vk::ImageViewCreateFlags(),
            image,
            vk::ImageViewType::e2D,
            format,
        };
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        vk::ImageView imageView;

        vk::resultCheck(device.createImageView(&viewInfo, nullptr, &imageView), "failed to create texture image view!");
        return imageView;
    }

    void VulkanRenderer::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage,
                                     vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory) {

        vk::ImageCreateInfo imageInfo {
            vk::ImageCreateFlags(),
            vk::ImageType::e2D,
            format,
            {width, height, 1},
            1,
            1,
            vk::SampleCountFlagBits::e1,
            tiling,
            usage,
            vk::SharingMode::eExclusive,
        };

        vk::resultCheck(device.createImage(&imageInfo, nullptr, &image), "failed to create image!");

        vk::MemoryRequirements memRequirements;
        device.getImageMemoryRequirements(image, &memRequirements);

        vk::MemoryAllocateInfo allocInfo {
            memRequirements.size,
            findMemoryType(memRequirements.memoryTypeBits, properties),
        };

        vk::resultCheck(device.allocateMemory(&allocInfo, nullptr, &imageMemory), "failed to allocate image memory!");
        vkBindImageMemory(device, image, imageMemory, 0);
    }


    void VulkanRenderer::createDepthResources() {
        vk::Format depthFormat = findDepthFormat();
        createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory);
        depthImageView = createImageView(depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
    }


    void VulkanRenderer::cleanupSwapChain() {

        device.destroyImageView(depthImageView, nullptr);
        device.destroyImage(depthImage, nullptr);
        device.freeMemory(depthImageMemory, nullptr);
        for (auto&& framebuffer : swapChainFramebuffers) {
            device.destroyFramebuffer(framebuffer, nullptr);
        }
        for (auto&& imageView : swapChainImageViews) {
            device.destroyImageView(imageView, nullptr);
        }
        device.destroySwapchainKHR(swapChain, nullptr);
    }

    VulkanRenderer::~VulkanRenderer() {
        
        device.waitIdle();
        cleanupSwapChain();

        device.destroyPipeline(graphicsPipeline, nullptr);
        device.destroyPipelineLayout(pipelineLayout, nullptr);
        device.destroyRenderPass(renderPass, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            device.destroyBuffer(uniformBuffers[i], nullptr);
            device.freeMemory(uniformBuffersMemory[i], nullptr);
        }
        device.destroyDescriptorPool(descriptorPool, nullptr);
        device.destroyDescriptorSetLayout(descriptorSetLayout, nullptr);
        device.destroyBuffer(indexBuffer, nullptr);
        device.freeMemory(indexBufferMemory, nullptr);
        device.destroyBuffer(vertexBuffer, nullptr);
        device.freeMemory(vertexBufferMemory, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            device.destroySemaphore(renderFinishedSemaphores[i], nullptr);
            device.destroySemaphore(imageAvailableSemaphores[i], nullptr);
            device.destroyFence(inFlightFences[i], nullptr);
        }

        device.destroyCommandPool(commandPool, nullptr);
        device.destroy();
        if (ValidationLayer::enable) {
            DebugMessenger::destroy(instance, debugMessenger);
        }
        instance.destroySurfaceKHR(surface, nullptr);
    }

    void VulkanRenderer::recreateSwapChain() {
        int width = 0, height = 0;
        window->getFramebufferSize(&width, &height);
        while (width == 0 || height == 0) {
            window->getFramebufferSize(&width, &height);
            glfwWaitEvents();
        }

        device.waitIdle();
        cleanupSwapChain();

        createSwapChain();
        createImageViews();
        createDepthResources();
        createFramebuffers();
    }


    void VulkanRenderer::createSurface() {
        VkSurfaceKHR surfaceCore {};
        if (window->createWindowSurface(instance, &surfaceCore) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
        surface = static_cast<vk::SurfaceKHR>(surfaceCore);
    }


    void VulkanRenderer::createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());
        for (uint32_t i = 0; i < swapChainImages.size(); i++) {
            swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, vk::ImageAspectFlagBits::eColor);
        }
    }

    void VulkanRenderer::createRenderPass() {
        vk::AttachmentDescription colorAttachment{
            vk::AttachmentDescriptionFlags(),
            swapChainImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        };

        vk::AttachmentReference colorAttachmentRef[] {{
            0, vk::ImageLayout::eColorAttachmentOptimal
        }};

        vk::AttachmentDescription depthAttachment {
            vk::AttachmentDescriptionFlags(),
            findDepthFormat(),
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::AttachmentReference depthAttachmentRef{
            1, vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        vk::SubpassDescription subpass[] {{
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            nullptr,
            colorAttachmentRef,
            nullptr,
            &depthAttachmentRef
        }};

        vk::SubpassDependency dependency[] {{
            VK_SUBPASS_EXTERNAL,
            0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
            vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
            vk::AccessFlags(),
            vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
        }};

        std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
        vk::RenderPassCreateInfo renderPassInfo {
            vk::RenderPassCreateFlags(),
            attachments,
            subpass,
            dependency
        };

        vk::resultCheck(device.createRenderPass(&renderPassInfo, nullptr, &renderPass), "failed to create render pass!");
    }

    void VulkanRenderer::createDescriptorSetLayout() {
        vk::DescriptorSetLayoutBinding uboLayoutBinding {
            0,
            vk::DescriptorType::eUniformBuffer,
            1,
            vk::ShaderStageFlagBits::eVertex,
            nullptr,
        };

        vk::DescriptorSetLayoutCreateInfo layoutInfo {
            vk::DescriptorSetLayoutCreateFlags(),
            1,
            &uboLayoutBinding
        };

        vk::resultCheck(device.createDescriptorSetLayout(&layoutInfo, nullptr, &descriptorSetLayout), "failed to create descriptor set layout!");
    }

    void VulkanRenderer::createGraphicsPipeline() {
        auto vertShaderCode = readFile(VERT_SHADER_PATH);
        auto fragShaderCode = readFile(FRAG_SHADER_PATH);

        vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        vk::PipelineShaderStageCreateInfo vertShaderStageInfo {
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eVertex,
            vertShaderModule,
            "main"
        };

        vk::PipelineShaderStageCreateInfo fragShaderStageInfo {
                vk::PipelineShaderStageCreateFlags(),
                vk::ShaderStageFlagBits::eFragment,
                fragShaderModule,
                "main"
        };

        vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        auto&& bindingDescription = Vertex::getBindingDescription();
        auto&& attributeDescriptions = Vertex::getAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo {
            vk::PipelineVertexInputStateCreateFlags(),
            1,
            &bindingDescription,
            static_cast<uint32_t>(attributeDescriptions.size()),
            attributeDescriptions.data()
        };

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly {
            vk::PipelineInputAssemblyStateCreateFlags(),
            vk::PrimitiveTopology::eTriangleList,
            VK_FALSE
        };

        vk::PipelineViewportStateCreateInfo viewportState {
            vk::PipelineViewportStateCreateFlags(),
            1,
            nullptr,
            1,
        };

        vk::PipelineRasterizationStateCreateInfo rasterizer {
            vk::PipelineRasterizationStateCreateFlags(),
            VK_FALSE,
            VK_FALSE,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
        };
        rasterizer.lineWidth = 1.0f;

        vk::PipelineMultisampleStateCreateInfo multisampling;

        vk::PipelineDepthStencilStateCreateInfo depthStencil {
            vk::PipelineDepthStencilStateCreateFlags(),
            VK_TRUE,
            VK_TRUE,
            vk::CompareOp::eLess,
            VK_FALSE,
        };

        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

        vk::PipelineColorBlendStateCreateInfo colorBlending {
            vk::PipelineColorBlendStateCreateFlags(),
            VK_FALSE,
            vk::LogicOp::eCopy,
            1,
            &colorBlendAttachment
        };

        std::vector<vk::DynamicState> dynamicStates = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
        };

        vk::PipelineDynamicStateCreateInfo dynamicState {
            vk::PipelineDynamicStateCreateFlags(),
            dynamicStates
        };

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo {
            vk::PipelineLayoutCreateFlags(),
            1,
            &descriptorSetLayout
        };

        vk::resultCheck(device.createPipelineLayout(&pipelineLayoutInfo, nullptr, &pipelineLayout), "failed to create pipeline layout!");

        vk::GraphicsPipelineCreateInfo pipelineInfo {
            vk::PipelineCreateFlags(),
            2,
            shaderStages,
            &vertexInputInfo,
            &inputAssembly,
            nullptr,
            &viewportState,
            &rasterizer,
            &multisampling,
            &depthStencil,
            &colorBlending,
            &dynamicState,
            pipelineLayout,
            renderPass
        };

        vk::resultCheck(device.createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline), "failed to create graphics pipeline!");

        device.destroy(fragShaderModule);
        device.destroy(vertShaderModule);
    }

    void VulkanRenderer::createFramebuffers() {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            std::array<vk::ImageView, 2> attachments = {
                swapChainImageViews[i],
                depthImageView
            };

            vk::FramebufferCreateInfo framebufferInfo {
                vk::FramebufferCreateFlags(),
                renderPass,
                attachments,
                swapChainExtent.width,
                swapChainExtent.height,
                1
            };

            vk::resultCheck(device.createFramebuffer(&framebufferInfo, nullptr, &swapChainFramebuffers[i]), "failed to create framebuffer!");
        }
    }

    void VulkanRenderer::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
        vk::CommandPoolCreateInfo poolInfo {
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueFamilyIndices.graphicsFamily.value()
        };
        vk::resultCheck(device.createCommandPool(&poolInfo, nullptr, &commandPool), "failed to create graphics command pool!");
    }

    void VulkanRenderer::createVertexBuffer() {
        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible| vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);


        auto&& data = device.mapMemory(stagingBufferMemory, 0, bufferSize);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        device.unmapMemory(stagingBufferMemory);

        createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void VulkanRenderer::createIndexBuffer() {
        vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

        auto&& data = device.mapMemory(stagingBufferMemory, 0, bufferSize);
        memcpy(data, indices.data(), (size_t) bufferSize);
        device.unmapMemory(stagingBufferMemory);

        createBuffer(bufferSize,  vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal,indexBuffer, indexBufferMemory);

        copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        device.destroyBuffer(stagingBuffer);
        device.freeMemory(stagingBufferMemory);
    }

    void VulkanRenderer::createUniformBuffers() {
        vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, uniformBuffers[i], uniformBuffersMemory[i]);
            vk::resultCheck(device.mapMemory(uniformBuffersMemory[i], 0, bufferSize, vk::MemoryMapFlags (), &uniformBuffersMapped[i]), "failed to map memory");
        }
    }

    void VulkanRenderer::createDescriptorPool() {
        vk::DescriptorPoolSize poolSize {
            vk::DescriptorType::eUniformBuffer,
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
        };

        vk::DescriptorPoolCreateInfo poolInfo {
            vk::DescriptorPoolCreateFlags(),
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
            1,
            &poolSize
        };

        vk::resultCheck(device.createDescriptorPool(&poolInfo, nullptr, &descriptorPool), "failed to create descriptor pool!");
    }

    void VulkanRenderer::createDescriptorSets() {
        std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        vk::DescriptorSetAllocateInfo allocInfo {
            descriptorPool,
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
            layouts.data()
        };

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        vk::resultCheck(device.allocateDescriptorSets(&allocInfo, descriptorSets.data()), "failed to allocate descriptor sets!");

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vk::DescriptorBufferInfo bufferInfo {
                uniformBuffers[i],
                0,
                sizeof(UniformBufferObject)
            };

            vk::WriteDescriptorSet descriptorWrite {
                descriptorSets[i],
                0,
                0,
                1,
                vk::DescriptorType::eUniformBuffer,
                nullptr,
                &bufferInfo
            };

            device.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
        }
    }

    void VulkanRenderer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) {
        
        vk::BufferCreateInfo bufferInfo {
            vk::BufferCreateFlags(),
            size,
            usage,
        };

        vk::resultCheck(device.createBuffer(&bufferInfo, nullptr, &buffer), "failed to create buffer!");

        vk::MemoryRequirements memRequirements;
        device.getBufferMemoryRequirements(buffer, &memRequirements);

        vk::MemoryAllocateInfo allocInfo {
            memRequirements.size,
            findMemoryType(memRequirements.memoryTypeBits, properties)
        };

        vk::resultCheck(device.allocateMemory(&allocInfo, nullptr, &bufferMemory), "failed to allocate buffer memory!");
        device.bindBufferMemory(buffer, bufferMemory, 0);
    }


    void VulkanRenderer::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) {
        vk::CommandBufferAllocateInfo allocInfo {
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            1
        };

        vk::CommandBuffer commandBuffer;
        vk::resultCheck(device.allocateCommandBuffers(&allocInfo, &commandBuffer), "failed to allocate command buffer");

        vk::CommandBufferBeginInfo beginInfo {
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit
        };

        commandBuffer.begin(beginInfo);
            vk::BufferCopy copyRegion {0, 0, size};
            commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
        commandBuffer.end();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vk::resultCheck(graphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE), "failed to submit graphics queue");
        graphicsQueue.waitIdle();
        device.freeCommandBuffers(commandPool, 1, &commandBuffer);
        }

    uint32_t VulkanRenderer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        vk::PhysicalDeviceMemoryProperties memProperties;
        physicalDevice.getMemoryProperties(&memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    void VulkanRenderer::createCommandBuffers() {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        vk::CommandBufferAllocateInfo allocInfo {
            commandPool,
            vk::CommandBufferLevel::ePrimary,
            static_cast<uint32_t>(commandBuffers.size())
        };

        vk::resultCheck(device.allocateCommandBuffers(&allocInfo, commandBuffers.data()), "failed to allocate command buffers!");
    }

    void VulkanRenderer::recordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex) {
        vk::CommandBufferBeginInfo beginInfo{};
        vk::resultCheck(commandBuffer.begin(&beginInfo), "failed to begin recording command buffer!");

        std::array<vk::ClearValue, 2> clearValues{};

        clearValues[0].setColor({0.005f, 0.005f, 0.009f, 1.0f});
        clearValues[1].setDepthStencil({1.0f, 0});

        vk::RenderPassBeginInfo renderPassInfo {
            renderPass,
            swapChainFramebuffers[imageIndex],
            {{0, 0}, swapChainExtent},
            clearValues
        };

        commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

            vk::Viewport viewport {
                0.0f, 0.0f,
                static_cast<float>(swapChainExtent.width),
                static_cast<float>(swapChainExtent.height),
                0.0f, 1.0f
            };

            commandBuffer.setViewport(0, 1, &viewport);
            vk::Rect2D scissor {
                {0, 0},
                swapChainExtent
            };

            commandBuffer.setScissor(0, 1, &scissor);

            vk::Buffer vertexBuffers[] = {vertexBuffer};
            vk::DeviceSize offsets[] = {0};
            commandBuffer.bindVertexBuffers( 0, 1, vertexBuffers, offsets);
            commandBuffer.bindIndexBuffer( indexBuffer, 0, vk::IndexType::eUint16);
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
            commandBuffer.drawIndexed( static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }

    void VulkanRenderer::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        vk::SemaphoreCreateInfo semaphoreInfo{};
        vk::FenceCreateInfo fenceInfo {
            vk::FenceCreateFlagBits::eSignaled
        };

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (device.createSemaphore(&semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != vk::Result::eSuccess ||
                device.createSemaphore(&semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != vk::Result::eSuccess ||
                device.createFence(&fenceInfo, nullptr, &inFlightFences[i]) != vk::Result::eSuccess) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void VulkanRenderer::updateUniformBuffer(uint32_t currentImage, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera_pos) {
        UniformBufferObject ubo{};
        ubo.view_pos = camera_pos;
        ubo.view = view;
        ubo.proj = proj;
        ubo.model =glm::mat4(1.0f);
        ubo.proj[1][1] *= -1;
        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    void VulkanRenderer::drawFrame(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera_pos) {
        vk::resultCheck(device.waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX), "failed to wait for fences");

        uint32_t imageIndex;
        auto&& result = device.acquireNextImageKHR(swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == vk::Result::eErrorOutOfDateKHR) {
            recreateSwapChain();
            return;
        } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(currentFrame, view, proj,camera_pos);

        vk::resultCheck(device.resetFences(1, &inFlightFences[currentFrame]), "failed to reset fences");

        commandBuffers[currentFrame].reset();
        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

        vk::Semaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::Semaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};

        vk::SubmitInfo submitInfo {
            1, waitSemaphores, waitStages,
            1, &commandBuffers[currentFrame],
            1, signalSemaphores
        };

        vk::resultCheck(graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame]), "failed to submit draw command buffer!");

        vk::SwapchainKHR swapChains[] = {swapChain};
        uint32_t imageIndexes[] = {imageIndex};

        vk::PresentInfoKHR presentInfo {
            signalSemaphores,
            swapChains,
            imageIndexes,
        };

        result = presentQueue.presentKHR(&presentInfo);
        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain();
        } else vk::resultCheck(result, "failed to present swap chain image!");

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    vk::ShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code) const {
        vk::ShaderModuleCreateInfo createInfo {
            vk::ShaderModuleCreateFlags(),
            code.size(),
            reinterpret_cast<const uint32_t*>(code.data())
        };

        vk::ShaderModule shaderModule;
        vk::resultCheck(device.createShaderModule(&createInfo, nullptr, &shaderModule), "failed to create shader module!");
        return shaderModule;
    }
}
