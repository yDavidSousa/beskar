#ifndef BESKAR_ENGINE_RESOURCE_SYSTEM_H
#define BESKAR_ENGINE_RESOURCE_SYSTEM_H

#include "beskar_engine/texture.h"
#include "beskar_engine/shader.h"

#include <unordered_map>
#include <vector>
#include <string>

enum class resource_type
{
    UNKNOWN,
    TEXTURE,
    SHADER
};

struct sprite_metadata
{
    unsigned int uid;
    std::string name;

    float x, y;
    int w, h;
};

struct texture_metadata
{
    unsigned int uid;
    const char* name;

    //data
    std::string full_path;
    wrap_mode wrap_mode;
    filter_mode filter_mode;
    pixel_format pixel_format;

    //runtime
    unsigned int handle_index;
    int count_refs;
};

struct shader_metadata
{
    unsigned int uid;
    const char* name;

    std::string full_path;

    const char* vert_source;
    const char* frag_source;
};

class resource_system
{
public:
    resource_system(const char* path);
    ~resource_system();

    [[nodiscard]] unsigned int load_texture(const char* path);
    texture* lookup_texture(unsigned int handle);
    void unload_texture(unsigned int handle);

    [[nodiscard]] unsigned int load_shader(const char* path);
    shader* lookup_shader(unsigned int handle);
    void unload_shader(unsigned int handle){};

    sprite_metadata lookup_sprite(const char* path, int index);
private:
    std::unordered_map<std::string, texture_metadata> _texture_cache;
    std::vector<texture*> _textures_loaded;

    std::unordered_map<std::string, std::vector<sprite_metadata>> _sprite_cache;

    std::unordered_map<std::string, shader_metadata> _shader_cache;
    std::vector<shader*> _shaders_loaded;
};

#endif //BESKAR_ENGINE_RESOURCE_SYSTEM_H