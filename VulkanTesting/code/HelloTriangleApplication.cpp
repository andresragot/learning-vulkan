//
//  HelloTriangleApplication.cpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 15/10/25.
//

#include "HelloTriangleApplication.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstring>
#include <set>
#include <fstream>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include "ObjC-interface.h"
#include "Assets.hpp"

namespace Ragot
{
    void HelloTriangleApplication::initWindow()
    {
        glfwInit();
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }
    
    void HelloTriangleApplication::mainLoop()
    {
        while (not glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }
    
    void HelloTriangleApplication::cleanup()
    {
        for (auto framebuffer : swapChainFrameBuffers)
        {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
    
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
     
        for (auto imageView : swapChainImageViews)
        {
            vkDestroyImageView(device, imageView, nullptr);
        }
    
        vkDestroySwapchainKHR(device, swapChain, nullptr);
    
        vkDestroyDevice(device, nullptr);
        
        vkDestroySurfaceKHR(vk_instance, surface, nullptr);
        
        vkDestroyInstance(vk_instance, nullptr);

        glfwDestroyWindow(window);
        
        glfwTerminate();
    }
    
    void HelloTriangleApplication::createInstance()
    {
        if (enable_validation_layers && not checkValidationLayerSupport())
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }
    
        VkApplicationInfo app_info {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "Hello Triangle";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "No Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;
        
        VkInstanceCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        
        uint32_t glfwExtensionCount = 0;
        const char ** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
        std::cout << "\nGLFW required extensions count: " << glfwExtensionCount << std::endl;
        
        std::vector<const char*> extensions;
        
        if (glfwExtensions && glfwExtensionCount > 0)
        {
            extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
            std::cout << "GLFW extensions:" << std::endl;
            for (uint32_t i = 0; i < glfwExtensionCount; ++i)
            {
                std::cout << "  - " << glfwExtensions[i] << std::endl;
            }
        }
        else
        {
            std::cout << "WARNING: GLFW returned no extensions, adding manually for macOS" << std::endl;
            extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);           // "VK_KHR_surface"
            #ifdef VK_USE_PLATFORM_MACOS_MVK
                // extensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME); // "VK_EXT_metal_surface"
                // O alternativamente:
                extensions.push_back("VK_MVK_macos_surface");
                extensions.push_back("VK_MVK_moltenvk");
            #endif
        }
        
        // Verificar extensiones disponibles
        uint32_t availableExtCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &availableExtCount, nullptr);
        std::vector<VkExtensionProperties> availableExts(availableExtCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &availableExtCount, availableExts.data());
        
        bool hasPortabilityEnum = false;
        for (const auto& ext : availableExts)
        {
            if (strcmp(ext.extensionName, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) == 0)
            {
                hasPortabilityEnum = true;
                break;
            }
        }
        
        // Solo agregar portability enumeration si está disponible
        if (hasPortabilityEnum)
        {
            extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            std::cout << "\nAdding VK_KHR_portability_enumeration" << std::endl;
        }
        
        std::cout << "\nEnabled extensions:" << std::endl;
        for (const auto& ext : extensions)
        {
            std::cout << "  - " << ext << std::endl;
        }
        
        create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();
        
        // Solo habilitar el flag si tenemos la extensión
        if (hasPortabilityEnum)
        {
            create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        }
        
        if (enable_validation_layers)
        {
            create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
            create_info.ppEnabledLayerNames = validation_layers.data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }
        
        VkResult result;
        if ((result = vkCreateInstance(&create_info, nullptr, &vk_instance)) != VK_SUCCESS)
        {
            std::cout << "Error code: " << result << std::endl;
            throw std::runtime_error("failed to create instance!");
        }
        
        std::cout << "Vulkan instance created successfully!" << std::endl;
    }


    void HelloTriangleApplication::createSurface()
    {
        id windowHandle = glfwGetCocoaWindow(window);
        id viewHandle = (id) getViewFromNSWindowPointer(windowHandle);
        
        VkMacOSSurfaceCreateInfoMVK create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
        create_info.pNext = nullptr;
        create_info.flags = 0;
        create_info.pView = viewHandle;
        
        PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
        vkCreateMacOSSurfaceMVK = (PFN_vkCreateMacOSSurfaceMVK) vkGetInstanceProcAddr(vk_instance, "vkCreateMacOSSurfaceMVK");
        
        if (vkCreateMacOSSurfaceMVK(vk_instance, &create_info, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create surface!");
        }
    }
    
    void HelloTriangleApplication::createSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physical_device);
        
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapSurfaceFormat(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
        
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        
        VkSwapchainCreateInfoKHR create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = surface;
        
        create_info.minImageCount = imageCount;
        create_info.imageFormat = surfaceFormat.format;
        create_info.imageColorSpace = surfaceFormat.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
        QueueFamilyIndices indices = findQueueFamilies(physical_device);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        
        if (indices.graphicsFamily != indices.presentFamily)
        {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0; // Optional
            create_info.pQueueFamilyIndices = nullptr; // Optional
        }
        
        create_info.preTransform = swapChainSupport.capabilities.currentTransform;
        
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        
        create_info.presentMode = presentMode;
        create_info.clipped = VK_TRUE;
        
        create_info.oldSwapchain = VK_NULL_HANDLE;
        
        if (vkCreateSwapchainKHR(device, &create_info, nullptr, &swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }
        
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
        
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }
    
    void HelloTriangleApplication::createImageViews()
    {
        swapChainImageViews.resize(swapChainImages.size());
        
        for (size_t i = 0; i < swapChainImages.size(); ++i)
        {
            VkImageViewCreateInfo create_info {};
            
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = swapChainImages[i];
            
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = swapChainImageFormat;
            
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;
            
            if (vkCreateImageView(device, &create_info, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }
    
    void HelloTriangleApplication::createRenderPass()
    {
        VkAttachmentDescription colorAttachment {};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        VkAttachmentReference colorAttachmentRef {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        
        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        
        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create render pass!");
        }
    }
    
    void HelloTriangleApplication::createGraphicsPipeline()
    {
        auto vertShaderCode = readFile(assets.get_asset_path("Shaders/vert.spv"));
        auto fragShaderCode = readFile(assets.get_asset_path("Shaders/frag.spv"));
        
        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
        
        VkPipelineShaderStageCreateInfo vertShaderStateInfo{};
        vertShaderStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStateInfo.module = vertShaderModule;
        vertShaderStateInfo.pName = "main";
        
        VkPipelineShaderStageCreateInfo fragShaderStateInfo{};
        fragShaderStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStateInfo.module = fragShaderModule;
        fragShaderStateInfo.pName = "main";
        
        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStateInfo, fragShaderStateInfo};
        
        std::vector < VkDynamicState > dynamicsStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        
        VkPipelineDynamicStateCreateInfo dynamic_state {};
        dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamicsStates.size());
        dynamic_state.pDynamicStates = dynamicsStates.data();
        
        VkPipelineVertexInputStateCreateInfo vertex_input_info {};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_info.vertexBindingDescriptionCount = 0;
        vertex_input_info.pVertexBindingDescriptions = nullptr;
        vertex_input_info.vertexAttributeDescriptionCount = 0;
        vertex_input_info.pVertexAttributeDescriptions = nullptr;
        
        VkPipelineInputAssemblyStateCreateInfo input_assembly {};
        input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly.primitiveRestartEnable = VK_FALSE;
        
        VkViewport viewport {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChainExtent.width);
        viewport.height = static_cast<float>(swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        
        VkRect2D scissor {};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;
        
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
        
        VkPipelineRasterizationStateCreateInfo rasterizer {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
        
        VkPipelineMultisampleStateCreateInfo multisampling {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional
        
        VkPipelineColorBlendAttachmentState colorBlendAttachment {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
        
        VkPipelineColorBlendStateCreateInfo colorBlending {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        
        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
        
        
        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertex_input_info;
        pipelineInfo.pInputAssemblyState = &input_assembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamic_state;
        
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;
        
        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline!");
        }
        
        vkDestroyShaderModule(device, vertShaderModule, nullptr);
        vkDestroyShaderModule(device, fragShaderModule, nullptr);
    }
    
    void HelloTriangleApplication::createFramebuffers()
    {
        swapChainFrameBuffers.resize(swapChainImages.size());
        
        for (size_t i = 0; i < swapChainImages.size(); ++i)
        {
            VkImageView attachments[] = {
                swapChainImageViews[i]
            };
            
            VkFramebufferCreateInfo framebufferInfo {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;
            
            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFrameBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
    
    void HelloTriangleApplication::pickPhysicalDevice()
    {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);
        
        if (device_count == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        
        std::vector < VkPhysicalDevice > devices (device_count);
        vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());
        
        for (const auto & device : devices)
        {
            if (isDeviceSuitable(device))
            {
                std::cout << "Device is suitable" << std::endl;
                physical_device = device;
                break;
            }
        }
        
        if (physical_device == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }
    
    void HelloTriangleApplication::availableExtensions()
    {
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        
        std::vector < VkExtensionProperties > extensions(extension_count);
        
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
        
        std::cout << "available extensions:" << std::endl;
        
        for (const auto & extension : extensions)
        {
            std::cout << '\t' << extension.extensionName << std::endl;
        }
    }
    
    bool HelloTriangleApplication::checkValidationLayerSupport()
    {
        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        
        std::vector < VkLayerProperties > available_layers (layer_count);
        
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
        
        for (const char * layer_name : validation_layers)
        {
            bool layer_found = false;
            
            for (const auto & layer_properties : available_layers)
            {
                std::cout << layer_properties.layerName << std::endl;
                if (strcmp(layer_name, layer_properties.layerName) == 0)
                {
                    layer_found = true;
                    break;
                }
            }
            
            if (not layer_found) return false;
        }
        
        return true;
    }
    
    bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures(device, &device_features);
        
        
        std::cout << "\nDevice name: " << device_properties.deviceName << std::endl;

        // return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && device_features.geometryShader;

        QueueFamilyIndices indices = findQueueFamilies(device);
        
        bool extensionsSupported = checkDeviceExtensionSupport(device);
        
        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = not swapChainSupport.formats.empty() && not swapChainSupport.presentModes.empty();
        }
        
        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }
    
    bool HelloTriangleApplication::checkDeviceExtensionSupport (VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        
        std::vector < VkExtensionProperties > availableExtensions (extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
        
        std::set < std::string > requiredExtensions (deviceExtensions.begin(), deviceExtensions.end());
        
        for (const auto & extension : availableExtensions)
        {
            requiredExtensions.erase (extension.extensionName);
        }
        
        return requiredExtensions.empty();
    }
    
    QueueFamilyIndices HelloTriangleApplication::findQueueFamilies (VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;
        
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        
        std::vector < VkQueueFamilyProperties > queueFamilies (queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        
        int i = 0;
        for (const auto & queueFamily : queueFamilies)
        {
            
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }
            
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            
            if (presentSupport)
            {
                indices.presentFamily = i;
            }
            
            if (indices.isComplete())
            {
                break;
            }
            
            ++i;
        }
        
        return indices;
    }
    
    SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport (VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;
        
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
        
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        
        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }
        
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &presentModeCount, nullptr);
        
        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }
        
        
        return details;
    }
    
    VkSurfaceFormatKHR HelloTriangleApplication::chooseSwapSurfaceFormat (const std::vector < VkSurfaceFormatKHR > & availableFormats)
    {
        for (const auto & availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }
    
        return availableFormats[0];
    }
    
    VkPresentModeKHR HelloTriangleApplication::chooseSwapSurfaceFormat (const std::vector < VkPresentModeKHR > & availablePresentModes)
    {
        for (const auto & availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }
    
        return VK_PRESENT_MODE_FIFO_KHR;
    }
    
    VkExtent2D HelloTriangleApplication::chooseSwapExtent (const VkSurfaceCapabilitiesKHR & capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits < uint32_t >::max())
        {
            return capabilities.currentExtent;
        }
            
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.maxImageExtent.height, capabilities.maxImageExtent.height);
        
        return actualExtent;            
    }
    
    
    void HelloTriangleApplication::createLogicalDevice()
    {
        QueueFamilyIndices indices = findQueueFamilies(physical_device);
        
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queue_create_info {};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queueFamily;
            queue_create_info.queueCount = 1;
            
            queue_create_info.pQueuePriorities = &queuePriority;
            queue_create_infos.push_back(queue_create_info);
        
        }
    
        
        VkDeviceCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        
        VkPhysicalDeviceFeatures device_features {};
        create_info.pEnabledFeatures = &device_features;
        
        create_info.enabledExtensionCount = static_cast < uint32_t > (deviceExtensions.size());
        create_info.ppEnabledExtensionNames = deviceExtensions.data();
        
        if (enable_validation_layers)
        {
            create_info.enabledLayerCount = static_cast < uint32_t > (validation_layers.size());
            create_info.ppEnabledLayerNames = validation_layers.data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }
        
        if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }
        
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }
    
    std::vector<char> HelloTriangleApplication::readFile (const std::string & filename)
    {
        std::ifstream file (filename, std::ios::ate | std::ios::binary);
        
        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }
        
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::cout << "File: " << filename << ", of size: " << fileSize << " opened." << std::endl;
        
        std::vector<char> buffer(fileSize);
        
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        
        return buffer;
    }
    
    VkShaderModule HelloTriangleApplication::createShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());
        
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &create_info, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }
        
        return shaderModule;
    }
    
    

}
