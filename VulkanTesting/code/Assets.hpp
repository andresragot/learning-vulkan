//
//  Assets.hpp
//  FileToOBJ
//
//  Created by Andrés Ragot on 3/2/25.
//

#pragma once

#include <iostream>
#include <filesystem>
#include <string>

namespace Ragot
{
    using std::string;
    using std::filesystem::path;

    class Assets
    {
        path base_path = "./";

    public:

        static Assets & instance()
        {
            static Assets assets;
            return assets;
        }

    private:

        Assets() = default;
        Assets(const Assets&) = delete;
        Assets(const Assets&&) = delete;
        Assets& operator = (const Assets&) = delete;
        Assets& operator = (const Assets&&) = delete;

    public:

        void initialize(const string& executable_file_path)
        {
            base_path = path{ executable_file_path }.parent_path() / "assets";
        }

        path get_asset_path(const string & asset_name)
        {
            // std::cout << base_path/asset_name << std::endl;
            return base_path/asset_name;
        }

    };
    
    enum
    {
        RENDER_NONE,
        RENDER_REVOLUTION,
        RENDER_PATH,
        RENDER_MAX
    };
    
    struct coordinates_t
    {
        float x = 0;
        float y = 0;
    };
    
    struct mesh_info_t
    {
        uint8_t render_flag = RENDER_NONE;
        size_t vertex_amount = 0;
        std::vector < coordinates_t > coordinates;
    };
    
    extern Assets & assets;
}
