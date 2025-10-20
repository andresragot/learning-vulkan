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
        
        create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        
        create_info.enabledExtensionCount = glfwExtensionCount;
        create_info.ppEnabledExtensionNames = glfwExtensions;
        
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
                physical_device = device;
                break;
            }
        }
        
        if (physical_device == VK_NULL_HANDLE)
        {
            std::runtime_error("failed to find a suitable GPU!");
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
        
        
        std::cout << "Device name: " << device_properties.deviceName << std::endl;

        return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
                device_features.geometryShader;
    }
}
