#ifndef BESKAR_SPRITE_EDITOR_H
#define BESKAR_SPRITE_EDITOR_H

#include "beskar_engine/resource_system.h"

#include <glm/glm.hpp>
#include <vector>

std::vector<sprite_metadata> sprite_slice_count(int tex_width, int tex_height, int column, int row, glm::vec2 offset, glm::vec2 spacing)
{
    float width = (tex_width - (offset.x * 2) - (column - 1 * spacing.x)) / column;
    float height = (tex_height - (offset.y * 2) - (row - 1 * spacing.y)) / row;

    std::vector<sprite_metadata> sprites;
    float y = offset.y;
    for (int r = 0; r < row; r++)
    {
        float x = offset.x;
        for (int c = 0; c < column; c++)
        {
            sprite_metadata sprite;
            sprite.x = x;
            sprite.y = y;
            sprite.w = width;
            sprite.h = height;
            sprites.push_back(sprite);
            x = (x + width) + spacing.x;
        }
        y = (y + height) + spacing.y;
    }

    return sprites;
};

#endif //BESKAR_SPRITE_EDITOR_H