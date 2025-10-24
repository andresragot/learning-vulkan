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

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

// También necesitas incluir vulkan.h para los defines de extensiones:
#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

#include "QueueFamilyIndices.hpp"

namespace  Ragot
{
    class HelloTriangleApplication
    {
    private:
        static constexpr unsigned WIDTH = 800;
        static constexpr unsigned HEIGHT = 600;
        
        const std::vector < const char * > validation_layers = {
            "MoltenVK"
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
        }
        
        void mainLoop();
        
        void cleanup();
        
        void createInstance();
        
        void createSurface();
        
        void pickPhysicalDevice();
        
        void createLogicalDevice();

        void availableExtensions();
        
        bool checkValidationLayerSupport();
        
        bool isDeviceSuitable(VkPhysicalDevice device);
        
        QueueFamilyIndices findQueueFamilies (VkPhysicalDevice device);
    };
}

