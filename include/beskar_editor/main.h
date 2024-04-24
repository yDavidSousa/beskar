#ifndef BESKAR_EDITOR_MAIN_H
#define BESKAR_EDITOR_MAIN_H

#include "beskar_engine/texture.h"
#include "beskar_editor/sprite_editor.h"

#include <random>

#include <stb_image.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sys/stat.h>

void create_project_settings(std::filesystem::path app_path, std::filesystem::path project_path)
{
    std::ofstream project_settings (app_path / "project_settings.txt");
    {
        project_settings << "projectName: " << project_path.filename().string() << std::endl;
        project_settings << "projectPath: " << project_path.string() << std::endl;
    }
    project_settings.close();
}

void create_metadata(std::filesystem::path resources_dir, std::filesystem::path file_path)
{
    std::filesystem::path metadata_path(file_path.string() + ".meta");

    std::ofstream metadata (metadata_path);
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<size_t> dis(1, std::numeric_limits<size_t>::max());
        unsigned long long uid = dis(gen);
        metadata << "guid: " << uid << std::endl;

        std::string file_name = file_path.stem().string();
        metadata << "fileName: " << file_name << std::endl;

        std::string relative_path = file_path.string();
        relative_path.erase(0, resources_dir.string().size());
        metadata << "filePath: " << relative_path << std::endl;

        struct stat statFile;
        stat(metadata_path.string().c_str(), &statFile);
        metadata << "timeCreated: " << statFile.st_ctime << std::endl;

        std::string extension = file_path.extension().string();
        if(extension == ".png")
        {
            metadata << "textureImporter: " << std::endl;

            int width, height, channels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *data = stbi_load(file_path.string().c_str(), &width, &height, &channels, 0);
            if(!data)
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);

            metadata << "\twrapMode: " << static_cast<int>(wrap_mode::CLAMP) << std::endl;
            metadata << "\tfilterMode: " << static_cast<int>(filter_mode::NEAREST) << std::endl;
            metadata << "\tpixelFormat: " << channels << std::endl;

            std::vector<sprite_metadata> sprite_sheet;
            //sprite_sheet = sprite_slice_count(width, height, 16, 9, {1, 1}, {1, 1}); //tilesheet
            //sprite_sheet = sprite_slice_count(width, height, 8, 6, {0, 0}, {0, 0}); //spritesheet
            if (sprite_sheet.size() > 0)
            {
                metadata << "\tspriteSheet: " << sprite_sheet.size() << std::endl;
                for (int i = 0; i < sprite_sheet.size(); ++i)
                {
                    metadata << "\t\t" << file_name + "_" + std::to_string(i) << std::endl;
                    metadata << "\t\t\tx: " << sprite_sheet[i].x << std::endl;
                    metadata << "\t\t\ty: " << sprite_sheet[i].y << std::endl;
                    metadata << "\t\t\twidth: " << sprite_sheet[i].w << std::endl;
                    metadata << "\t\t\theight: " << sprite_sheet[i].h << std::endl;
                }
            }
        }
        else if(extension == ".glsl")
        {
            metadata << "shaderImporter: " << std::endl;
        }
    }
    metadata.close();
}

void write_cache(const char* path)
{
    std::filesystem::path current_path(path);
    std::filesystem::path app_path = current_path.parent_path();

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
    std::filesystem::path resources_path = current_path;
    std::filesystem::path project_path = current_path.parent_path();

    create_project_settings(app_path, project_path);
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{resources_path})
    {
        //std::cout << dir_entry << '\n';
        if (dir_entry.is_directory())
        {
            //std::cout << "is directory" << '\n';
            continue;
        }

        std::filesystem::path file_path = dir_entry.path();

#if __APPLE__
        if(file_path.filename() == ".DS_Store")
        {
            continue;
        }
#endif

        std::string extension = file_path.extension().string();
        if(extension == ".meta")
        {
            //std::cout << "is meta file" << '\n';
            continue;
        }

        if (std::filesystem::exists(file_path.string() + ".meta"))
        {
            //std::cout << "already has a meta file" << '\n';
            continue;
        }

        create_metadata(current_path, file_path);
    }
}

#endif //BESKAR_EDITOR_MAIN_H