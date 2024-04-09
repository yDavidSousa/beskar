#include "beskar_engine/resource_manager.h"
#include "beskar_engine/guid.h"

#include <iostream>
#include <filesystem>
#include <fstream>

int main(int argc, char ** argv)
{
    std::filesystem::path current_path = argv[0];
    while (current_path.empty() == false)
    {
        std::filesystem::path target_path = current_path / "data";
        if(std::filesystem::exists(target_path) && std::filesystem::is_directory(target_path))
        {
            current_path = target_path;
            break;
        }

        current_path = current_path.parent_path();
    }

    resource_manager rm;
    rm.data_path = current_path;

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{rm.data_path})
    {
        //std::cout << dir_entry << '\n';
        if (dir_entry.is_directory())
        {
            //std::cout << "is directory" << '\n';
            continue;
        }

        std::filesystem::path asset_path = dir_entry.path();

#if __APPLE__
        if(asset_path.filename() == ".DS_Store")
        {
            continue;
        }
#endif

        std::string extension = asset_path.extension().string();
        if(extension == ".meta")
        {
            //std::cout << "is meta file" << '\n';
            continue;
        }

        if (std::filesystem::exists(asset_path.string() + ".meta"))
        {
            //std::cout << "already has a meta file" << '\n';
            continue;
        }

        std::string filename = asset_path.stem().string();
        std::string filepath = asset_path.string();

        asset_path += ".meta";
        std::ofstream metadata (asset_path);
        {
            metadata << "metaversion: " << 0 << "\n";

            guid guid = guid_generator::new_guid();
            metadata << "guid: " << guid.str() << "\n";

            std::filesystem::file_time_type filetime = std::filesystem::last_write_time(asset_path);
            long int lastwritetime = filetime.time_since_epoch().count();
            metadata << "timecreated: " << lastwritetime << "\n";

            metadata << "name: " << filename << "\n";
            metadata << "file: " << filepath << "\n";
        }
        metadata.close();
    }

    return 0;
}