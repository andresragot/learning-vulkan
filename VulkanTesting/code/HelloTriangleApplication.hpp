//
//  HelloTriangleApplication.hpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 15/10/25.
//

#pragma once


// En HelloTriangleApplication.hpp, ANTES de incluir GLFW:
#define VK_USE_PLATFORM_MACOS_MVK
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

#include "SwapChainSupportDetails.hpp"
#include "QueueFamilyIndices.hpp"

namespace  Ragot
{
    class HelloTriangleApplication
    {
    private:
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
        const bool enable_validation_layers = true;
#endif

    
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
        }
        
        void mainLoop();
        
        void cleanup();
        
        void createInstance();
        
        void createSurface();
        
        void pickPhysicalDevice();
        
        void createLogicalDevice();
        
        void createSwapChain();
        
        void createImageViews();

        void availableExtensions();
        
        bool checkValidationLayerSupport();
        
        bool isDeviceSuitable(VkPhysicalDevice device);
        
        bool checkDeviceExtensionSupport (VkPhysicalDevice device);
        
        QueueFamilyIndices findQueueFamilies (VkPhysicalDevice device);
        
        SwapChainSupportDetails querySwapChainSupport (VkPhysicalDevice device);
        
        VkSurfaceFormatKHR chooseSwapSurfaceFormat (const std::vector < VkSurfaceFormatKHR > & availableFormats);
        
        VkPresentModeKHR chooseSwapSurfaceFormat (const std::vector < VkPresentModeKHR > & availablePresentModes);
        
        VkExtent2D chooseSwapExtent (const VkSurfaceCapabilitiesKHR & capabilities);
    };
}

