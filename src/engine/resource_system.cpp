#include "beskar_engine/resource_system.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
#include <fstream>
#include <iostream>

resource_system::resource_system(const char* path)
{
    std::filesystem::path current_path = path;
    current_path = current_path.parent_path();

    std::string project_name;
    std::string resource_path;

    std::filesystem::path project_settings_path = current_path / "project_settings.txt";
    std::ifstream project_settings (project_settings_path);
    {
        project_settings >> project_name >> project_name;
        project_settings >> resource_path >> resource_path;
    }
    project_settings.close();

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{resource_path})
    {
        if (dir_entry.is_directory()) continue;

        std::filesystem::path asset_path = dir_entry.path();
        std::string extension = asset_path.extension().string();
        if(extension != ".meta") continue;

        std::string buffer;
        std::ifstream metadata (asset_path);
        {
            unsigned long long uid;
            metadata >> buffer >> uid;
            long long time_created;
            metadata >> buffer >> time_created;
            std::string fileName;
            metadata >> buffer >> fileName;
            std::string filepath;
            metadata >> buffer >> filepath;

            metadata >> buffer;
            if(buffer == "textureImporter:")
            {
                texture_metadata texture_importer;
                texture_importer.uid = uid;
                int value;
                metadata >> buffer >> value;
                texture_importer.wrap_mode = static_cast<wrap_mode>(value);
                metadata >> buffer >> value;
                texture_importer.filter_mode = static_cast<filter_mode>(value);
                metadata >> buffer >> value;
                texture_importer.pixel_format = static_cast<pixel_format>(value);
                metadata >> buffer;

                std::vector<sprite_metadata> sprite_sheet;
                if(buffer == "spriteSheet:")
                {
                    int length;
                    metadata >> length;
                    for (int i = 0; i < length; ++i)
                    {
                        sprite_metadata sprite_metadata;
                        metadata >> sprite_metadata.name;
                        metadata >> buffer >> sprite_metadata.x;
                        metadata >> buffer >> sprite_metadata.y;
                        metadata >> buffer >> sprite_metadata.w;
                        metadata >> buffer >> sprite_metadata.h;
                        sprite_sheet.push_back(sprite_metadata);
                    }
                }
                texture_importer.full_path = filepath.c_str();
                texture_importer.count_refs = 0;

                _texture_cache[filepath.erase(0, resource_path.size() + 1)] = texture_importer;
                if(sprite_sheet.size() > 0)
                {
                    _sprite_cache[filepath] = sprite_sheet;
                }
            }
            else if(buffer == "shaderImporter:")
            {
                shader_metadata shader_metadata;
                shader_metadata.uid = uid;
                shader_metadata.full_path = filepath.c_str();

                _shader_cache[filepath.erase(0, resource_path.size() + 1)] = shader_metadata;
            }
        }
        metadata.close();
    }
}

resource_system::~resource_system(){}

unsigned int resource_system::load_texture(const char* path)
{
    if(_texture_cache.contains(path) == false)
        return {};

    texture_metadata metadata = _texture_cache[path];

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(metadata.full_path.c_str(), &width, &height, &channels, 0);
    if(!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    texture* texture_instance = new texture(width, height, metadata.wrap_mode, metadata.filter_mode, metadata.pixel_format, data);

    unsigned int handle = 0;
    unsigned short index = _textures_loaded.size();
    metadata.handle_index = index;
    _textures_loaded.push_back(texture_instance);

    stbi_image_free(data);
    metadata.count_refs++;

    handle |= index << 16;
    handle |= static_cast<unsigned short>(resource_type::TEXTURE);

    return handle;
}

void resource_system::unload_texture(unsigned int handle)
{
    unsigned short type = handle & 0xFFFF;
    if(type != static_cast<int>(resource_type::TEXTURE)) return;

    unsigned short index = (handle >> 16) & 0xFFFF;
    if(_textures_loaded.size() <= index) return;

    texture* texture_loaded = _textures_loaded[index];
    delete texture_loaded;
}

texture* resource_system::lookup_texture(unsigned int handle)
{
    unsigned short type = handle & 0xFFFF;
    if(type != static_cast<int>(resource_type::TEXTURE)) return nullptr;

    unsigned short index = (handle >> 16) & 0xFFFF;
    if(_textures_loaded.size() <= index) return nullptr;

    texture* texture_loaded = _textures_loaded[index];
    return texture_loaded;
}

sprite_metadata resource_system::lookup_sprite(const char* path, int index)
{
    if(_texture_cache.contains(path) == false)
        return {};

    std::vector<sprite_metadata> sprite_sheet = _sprite_cache[path];
    return sprite_sheet[index];
}

unsigned int resource_system::load_shader(const char* path)
{
    if(_shader_cache.contains(path) == false)
        return {};

    shader_metadata metadata = _shader_cache[path];

    std::ifstream file(metadata.full_path);
    if (file.is_open() == false)
    {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    const char vertex = 0;
    const char fragment = 1;

    char source_type;

    std::string buffer;
    std::string vert_source, frag_source;
    while (std::getline(file, buffer))
    {
        if (buffer.find("#vert") != std::string::npos)
        {
            source_type = vertex;
            continue;
        } else if (buffer.find("#frag") != std::string::npos)
        {
            source_type = fragment;
            continue;
        }

        switch (source_type)
        {
            case vertex:
                vert_source += buffer + "\n";
                break;
            case fragment:
                frag_source += buffer + "\n";
                break;
        }
    }

    shader* shader_instance = new shader(vert_source.c_str(), frag_source.c_str());
    unsigned short index = _shaders_loaded.size();
    _shaders_loaded.push_back(shader_instance);

    unsigned int handle = 0;
    handle |= index << 16;
    handle |= static_cast<unsigned short>(resource_type::SHADER);

    return handle;
}

shader* resource_system::lookup_shader(unsigned int handle)
{
    unsigned short type = handle & 0xFFFF;
    if(type != static_cast<int>(resource_type::SHADER)) return nullptr;

    unsigned short index = (handle >> 16) & 0xFFFF;
    if(_shaders_loaded.size() <= index) return nullptr;

    shader* shader_loaded = _shaders_loaded[index];
    return shader_loaded;
}