#include "beskar_engine/sprite_asset.h"

sprite_asset::sprite_asset() : x(0), y(0), w(0), h(0)
{
}

sprite_asset::sprite_asset(float x, float y, int w, int h) : x(x), y(y), w(w), h(h)
{
}

std::vector<sprite_asset> sprite_slice_size(const gl_texture* texture, int width, int height, glm::vec2 offset, glm::vec2 spacing)
{
    int tex_width = texture->get_width();
    int tex_height = texture->get_height();

    int column = static_cast<int>((tex_width - (offset.x * 2) - (column - 1 * spacing.x)) / width);
    int row = static_cast<int>((tex_height - (offset.y * 2) - (row - 1 * spacing.y)) / height);

    std::vector<sprite_asset> sprite_assets;
    float y = offset.y;
    for (int r = 0; r < row; r++)
    {
        float x = offset.x;
        for (int c = 0; c < column; c++)
        {
            sprite_asset sprite_asset(x, y, width, height);

            float left = x / tex_width;
            float right = y / tex_height;
            float bottom = (x + width) / tex_width;
            float top = (y + height) / tex_height;

            sprite_asset.tex_coords = new float[]{
                    left, bottom,
                    right, bottom,
                    right, top,
                    left, top
            };
            sprite_assets.push_back(sprite_asset);

            x = (x + width) + spacing.x;
        }

        y = (y + height) + spacing.y;
    }

    return sprite_assets;
}

std::vector<sprite_asset> sprite_slice_count(const gl_texture* texture, int column, int row, glm::vec2 offset, glm::vec2 spacing)
{
    int tex_width = texture->get_width();
    int tex_height = texture->get_height();

    float width = (tex_width - (offset.x * 2) - (column - 1 * spacing.x)) / column;
    float height = (tex_height - (offset.y * 2) - (row - 1 * spacing.y)) / row;

    std::vector<sprite_asset> sprite_assets;
    float y = offset.y;
    for (int r = 0; r < row; r++)
    {
        float x = offset.x;
        for (int c = 0; c < column; c++)
        {
            sprite_asset sprite_asset(x, y, width, height);

            float left = x / tex_width;
            float right = (x + width) / tex_width;
            float bottom = y / tex_height;
            float top = (y + height) / tex_height;

            sprite_asset.tex_coords = new float[]{
                left, bottom,
                right, bottom,
                right, top,
                left, top
            };
            sprite_assets.push_back(sprite_asset);

            x = (x + width) + spacing.x;
        }

        y = (y + height) + spacing.y;
    }

    return sprite_assets;
}