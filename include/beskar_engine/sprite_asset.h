#ifndef BESKAR_SPRITE_ASSET_H
#define BESKAR_SPRITE_ASSET_H

#include "gl_texture.h"

#include "glm/glm.hpp"
#include <vector>

struct sprite_asset
{
    float x, y;
    int w, h;
    float* tex_coords;

    sprite_asset();
    sprite_asset(float x, float y, int w, int h);
};

std::vector<sprite_asset> sprite_slice_size(const gl_texture* texture, int width, int height, glm::vec2 offset, glm::vec2 spacing);
std::vector<sprite_asset> sprite_slice_count(const gl_texture* texture, int column, int row, glm::vec2 offset, glm::vec2 spacing);

#endif //BESKAR_SPRITE_ASSET_H