//
//  frag.spv
//  VulkanTesting
//
//  Created by Andrés Ragot on 3/11/25.
//

#version 450

layout(location = 0) out vec4 outColor;
layout(location = 0) in  vec3 fragColor;

void main()
{
    outColor = vec4(fragColor, 1.0);
}
