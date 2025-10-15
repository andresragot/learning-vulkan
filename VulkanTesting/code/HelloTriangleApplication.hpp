//
//  HelloTriangleApplication.hpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 15/10/25.
//

#pragma once

#include <vulkan/vulkan.h>

namespace  Ragot
{
    class HelloTriangleApplication
    {
    public:
        void run ()
        {
            initVulkan ();
            mainLoop ();
            cleanup ();
        }
        
    private:
        void initVulkan()
        {
        
        }
        
        void mainLoop()
        {
        
        }
        
        void cleanup()
        {
        
        }
    };
}

