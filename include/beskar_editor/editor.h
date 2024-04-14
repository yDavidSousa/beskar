#ifndef BESKAR_EDITOR_H
#define BESKAR_EDITOR_H

#include <iostream>
#include <filesystem>
#include <fstream>

void cache_update(const char* path)
{
    std::filesystem::path current_path = path;
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

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{current_path})
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

            std::filesystem::file_time_type filetime = std::filesystem::last_write_time(asset_path);
            long int lastwritetime = filetime.time_since_epoch().count();
            metadata << "timecreated: " << lastwritetime << "\n";

            metadata << "name: " << filename << "\n";
            metadata << "file: " << filepath << "\n";
        }
        metadata.close();
    }

    std::filesystem::path build_path = path;
    build_path = build_path.parent_path();

    std::ofstream project_settings (build_path / "project_settings.txt");
    {
        project_settings << current_path.parent_path().filename() << std::endl;
        project_settings << current_path << std::endl;
    }
    project_settings.close();
}

#endif //BESKAR_EDITOR_H