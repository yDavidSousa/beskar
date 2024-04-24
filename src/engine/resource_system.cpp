#include "beskar_engine/resource_system.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>
#include <iostream>

resource_system::resource_system(const char* path)
{
    std::filesystem::path current_path = path;
    current_path = current_path.parent_path();

    std::string buffer;

    std::string project_name;
    std::filesystem::path project_path;

    std::filesystem::path project_settings_path = current_path / "project_settings.txt";
    std::ifstream project_settings (project_settings_path);
    {
        project_settings >> buffer >> project_name;
        project_settings >> buffer >> project_path;
    }
    project_settings.close();

    resource_path = project_path / "data";

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{resource_path})
    {
        if (dir_entry.is_directory()) continue;

        std::filesystem::path asset_path = dir_entry.path();
        std::string extension = asset_path.extension().string();
        if(extension != ".meta") continue;

        std::ifstream metadata (asset_path);
        {
            resource_metadata resource;

            unsigned long long uid;
            metadata >> buffer >> uid;
            long long time_created;
            metadata >> buffer >> time_created;
            std::string file_name;
            metadata >> buffer >> file_name;
            std::string file_relative_path;
            metadata >> buffer >> file_relative_path;

            metadata >> buffer;
            if(buffer == "textureImporter:")
            {
                resource.type = resource_type::TEXTURE;

                texture_metadata texture_importer;
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
                        metadata >> buffer;
                        metadata >> buffer >> sprite_metadata.x;
                        metadata >> buffer >> sprite_metadata.y;
                        metadata >> buffer >> sprite_metadata.w;
                        metadata >> buffer >> sprite_metadata.h;
                        sprite_sheet.push_back(sprite_metadata);
                    }
                }

                _texture_cache[uid] = texture_importer;
                if(sprite_sheet.size() > 0)
                {
                    _sprite_cache[uid] = sprite_sheet;
                }
            }
            else if(buffer == "shaderImporter:")
            {
                resource.type = resource_type::SHADER;
            }
            resource.uid = uid;
            resource.relative_path = file_relative_path;
            resource.name = file_name;
            resource.refs_count = 0;

            _path_guid_pair[file_relative_path] = uid;
            _guid_resource_pair[uid] = resource;
        }
        metadata.close();
    }
}

resource_system::~resource_system(){}

unsigned int resource_system::load_texture(const char* path)
{
    if(_path_guid_pair.contains(path) == false)
        return {};

    unsigned long long uid = _path_guid_pair[path];
    if(_guid_resource_pair.contains(uid) == false)
        return {};

    resource_metadata resource_metadata = _guid_resource_pair[uid];
    texture_metadata metadata = _texture_cache[uid];

    std::filesystem::path file_path = resource_path / resource_metadata.relative_path;

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file_path.string().c_str(), &width, &height, &channels, 0);
    if(!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    texture* texture_instance = new texture(width, height, metadata.wrap_mode, metadata.filter_mode, metadata.pixel_format, data);
    resource_metadata.refs_count++;
    _guid_resource_pair[uid] = resource_metadata;

    unsigned int handle = 0;
    unsigned short index = static_cast<unsigned short>(_textures_loaded.size());
    _textures_loaded.push_back(texture_instance);

    stbi_image_free(data);

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
    if(_path_guid_pair.contains(path) == false)
        return {};

    unsigned long long uid = _path_guid_pair[path];
    if(_guid_resource_pair.contains(uid) == false)
        return {};

    if(_texture_cache.contains(uid) == false)
        return {};

    std::vector<sprite_metadata> sprite_sheet = _sprite_cache[uid];
    return sprite_sheet[index];
}

unsigned int resource_system::load_shader(const char* path)
{
    if(_path_guid_pair.contains(path) == false)
        return {};

    unsigned long long uid = _path_guid_pair[path];
    if(_guid_resource_pair.contains(uid) == false)
        return {};

    resource_metadata metadata = _guid_resource_pair[uid];

    std::filesystem::path file_path = resource_path / metadata.relative_path;
    std::ifstream file(file_path);
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

    metadata.refs_count++;
    _guid_resource_pair[uid] = metadata;

    shader* shader_instance = new shader(vert_source.c_str(), frag_source.c_str());
    unsigned short index = static_cast<unsigned short>(_shaders_loaded.size());
    _shaders_loaded.push_back(shader_instance);

    unsigned int handle = 0;
    handle |= index << 16;
    handle |= static_cast<unsigned short>(resource_type::SHADER);

    metadata.refs_count++;

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