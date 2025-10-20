//
//  HelloTriangleApplication.hpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 15/10/25.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace  Ragot
{
    class HelloTriangleApplication
    {
    private:
        static constexpr unsigned WIDTH = 800;
        static constexpr unsigned HEIGHT = 600;
    
        GLFWwindow * window;
        VkInstance vk_instance;
    
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
        }
        
        void mainLoop();
        
        void cleanup();
        
        void createInstance();

        void availableExtensions();
    };
}

