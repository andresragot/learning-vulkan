//
//  HelloTriangleApplication.hpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 15/10/25.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

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
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    
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
            pickPhysicalDevice();
        }
        
        void mainLoop();
        
        void cleanup();
        
        void createInstance();
        
        void pickPhysicalDevice();

        void availableExtensions();
        
        bool checkValidationLayerSupport();
        
        bool isDeviceSuitable(VkPhysicalDevice device);
    };
}

