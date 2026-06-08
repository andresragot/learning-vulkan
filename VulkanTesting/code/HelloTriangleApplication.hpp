//
//  HelloTriangleApplication.hpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 15/10/25.
//

#pragma once


// En HelloTriangleApplication.hpp, ANTES de incluir GLFW:
#ifdef __APPLE__
    #define VK_USE_PLATFORM_MACOS_MVK
#endif
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>
#include <string>

#include "SwapChainSupportDetails.hpp"
#include "QueueFamilyIndices.hpp"

namespace  Ragot
{
    class HelloTriangleApplication
    {
    private:
        const int MAX_FRAMES_IN_FLIGHT = 2;

        static constexpr unsigned WIDTH = 800;
        static constexpr unsigned HEIGHT = 600;
        
        const std::vector < const char * > validation_layers = {
            "MoltenVK",
        };
        
        const std::vector < const char * > deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

#ifdef NDEBUG
        const bool enable_validation_layers = false;
#else
        const bool enable_validation_layers = false;
#endif
        uint32_t currentFrame = 0;
    
        GLFWwindow * window;
        VkInstance vk_instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        
        VkSwapchainKHR swapChain;
        std::vector < VkImage > swapChainImages;
        std::vector < VkImageView > swapChainImageViews;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkPipeline graphicsPipeline;
        
        std::vector < VkFramebuffer > swapChainFrameBuffers;
        
        VkCommandPool commandPool;
        std::vector < VkCommandBuffer > commandBuffers;
        
        std::vector < VkSemaphore > imageAvailableSemaphores;
        std::vector < VkSemaphore > renderFinishedSemaphores;
        std::vector < VkFence > inFlightFences;

    public:
        bool framebufferResized = false;
    
    public:
        void run ()
        {
            initWindow();
            initVulkan ();
            mainLoop ();
            cleanup ();
        }
        
    private:
        void initWindow();
    
        void initVulkan()
        {
            createInstance();
            createSurface();
            pickPhysicalDevice();
            createLogicalDevice();
            createSwapChain();
            createImageViews();
            createRenderPass();
            createGraphicsPipeline();
            createFramebuffers();
            createCommandPool();
            createCommandBuffers();
            createSyncObjects();
        }

        void recreateSwapChain();
        
        void mainLoop();
        
        void cleanup();

        void cleanupSwapChain();
        
        void createInstance();
        
        void createSurface();
        
        void pickPhysicalDevice();
        
        void createLogicalDevice();
        
        void createSwapChain();
        
        void createImageViews();
        
        void createRenderPass();
        
        void createGraphicsPipeline();
        
        void createFramebuffers();
        
        void createCommandPool();
        
        void createCommandBuffers();
        
        void createSyncObjects();

        void availableExtensions();
        
        bool checkValidationLayerSupport();
        
        bool isDeviceSuitable(VkPhysicalDevice device);
        
        bool checkDeviceExtensionSupport (VkPhysicalDevice device);
        
        QueueFamilyIndices findQueueFamilies (VkPhysicalDevice device);
        
        SwapChainSupportDetails querySwapChainSupport (VkPhysicalDevice device);
        
        VkSurfaceFormatKHR chooseSwapSurfaceFormat (const std::vector < VkSurfaceFormatKHR > & availableFormats);
        
        VkPresentModeKHR chooseSwapSurfaceFormat (const std::vector < VkPresentModeKHR > & availablePresentModes);
        
        VkExtent2D chooseSwapExtent (const VkSurfaceCapabilitiesKHR & capabilities);
        
        static std::vector<char> readFile (const std::string & filename);
        
        VkShaderModule createShaderModule(const std::vector<char>& code);
        
        void recordCommanBuffer (VkCommandBuffer commandBuffer, uint32_t imageIndex);
        
        void drawFrame();
    };
}

