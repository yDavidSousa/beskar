#ifndef BESKAR_ENGINE_RESOURCE_SYSTEM_H
#define BESKAR_ENGINE_RESOURCE_SYSTEM_H

#include "beskar_engine/texture.h"
#include "beskar_engine/shader.h"

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>

enum class resource_type
{
    UNKNOWN,
    TEXTURE,
    SHADER
};

struct resource_metadata
{
    unsigned long long uid;
    std::string name;
    std::string relative_path;
    resource_type type;
    int refs_count;
};

struct sprite_metadata
{
    float x, y;
    int w, h;
};

struct texture_metadata
{
    std::string full_path;
    wrap_mode wrap_mode;
    filter_mode filter_mode;
    pixel_format pixel_format;
};

class resource_system
{
public:
    resource_system(const char* path);
    ~resource_system();

    std::vector<unsigned long long> get_guids()
    {
        std::vector<unsigned long long> result;
        result.reserve(_guid_resource_pair.size());
        for (const auto&kvp : _guid_resource_pair)
        {
            result.push_back(kvp.first);
        }
        return result;
    };

    resource_metadata lookup_resource(unsigned long long uid)
    {
        if(_guid_resource_pair.contains(uid))
        {
            return _guid_resource_pair[uid];
        }

        return resource_metadata();
    };

    [[nodiscard]] unsigned int load_texture(const char* path);
    texture* lookup_texture(unsigned int handle);
    void unload_texture(unsigned int handle);

    [[nodiscard]] unsigned int load_shader(const char* path);
    shader* lookup_shader(unsigned int handle);
    void unload_shader(unsigned int handle){};

    sprite_metadata lookup_sprite(const char* path, int index);
private:
    std::filesystem::path resource_path;

    std::unordered_map<std::string, unsigned long long> _path_guid_pair;
    std::unordered_map<unsigned long long, resource_metadata> _guid_resource_pair;

    std::unordered_map<unsigned long long, texture_metadata> _texture_cache;
    std::unordered_map<unsigned long long, std::vector<sprite_metadata>> _sprite_cache;

    std::vector<texture*> _textures_loaded;
    std::vector<shader*> _shaders_loaded;
};

#endif //BESKAR_ENGINE_RESOURCE_SYSTEM_H