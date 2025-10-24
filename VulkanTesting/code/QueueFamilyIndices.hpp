//
//  QueueFamilyIndices.hpp
//  VulkanTesting
//
//  Created by Andrés Ragot on 21/10/25.
//
#pragma once
#include <optional>

namespace Ragot
{
    class QueueFamilyIndices
    {
    public:
        std::optional < uint32_t > graphicsFamily;
        std::optional < uint32_t > presentFamily;
        
        QueueFamilyIndices () = default;
       ~QueueFamilyIndices () = default;
       
    public:
        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
}

