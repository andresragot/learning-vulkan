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
        void initWindow()
        {
            glfwInit();
            
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            
            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        }
    
        void initVulkan()
        {
            createInstance();
        }
        
        void mainLoop()
        {
            while (not glfwWindowShouldClose(window))
            {
                glfwPollEvents();
            }
        }
        
        void cleanup()
        {
            glfwDestroyWindow(window);
            
            glfwTerminate();
        }
        
        void createInstance()
        {
            VkApplicationInfo app_info {};
            app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            app_info.pApplicationName = "Hello Triangle";
            app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            app_info.pEngineName = "No Engine";
            app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            app_info.apiVersion = VK_API_VERSION_1_0;
        }
    };
}

