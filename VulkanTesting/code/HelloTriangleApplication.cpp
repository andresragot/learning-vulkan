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
        vkDestroyInstance(vk_instance, nullptr);

        glfwDestroyWindow(window);
        
        glfwTerminate();
    }
    
    void HelloTriangleApplication::createInstance()
    {
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
        
        create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        
        create_info.enabledExtensionCount = glfwExtensionCount;
        create_info.ppEnabledExtensionNames = glfwExtensions;
        
        create_info.enabledLayerCount = 0;
        
        
        VkResult result;
        if ((result = vkCreateInstance(&create_info, nullptr, &vk_instance)) != VK_SUCCESS)
        {
            std::cout << "Error code: " << result << std::endl;
            throw std::runtime_error("failed to create instance!");
        }
        
        std::cout << "Vulkan instance created successfully!" << std::endl;
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
}
