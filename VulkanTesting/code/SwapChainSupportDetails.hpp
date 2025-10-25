//
//  SwapChainSupportDetails.hpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 25/10/25.
//

#pragma once

namespace Ragot
{
    class SwapChainSupportDetails
    {
    public:
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector < VkSurfaceFormatKHR > formats;
        std::vector < VkPresentModeKHR > presentModes;
        
    public:
        SwapChainSupportDetails() = default;
       ~SwapChainSupportDetails() = default;
    };
}
