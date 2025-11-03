#include "HelloTriangleApplication.hpp"

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "Assets.hpp"

int main (int argc, const char * argv[])
{
    std::cout << argv[0] << std::endl;
    
    Ragot::assets.initialize(argv[0]);

    Ragot::HelloTriangleApplication app;
    
    try
    {
        app.run();
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

