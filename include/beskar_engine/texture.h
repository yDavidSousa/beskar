#ifndef BESKAR_ENGINE_TEXTURE_H
#define BESKAR_ENGINE_TEXTURE_H

enum class wrap_mode
{
    CLAMP = 0
};

enum class filter_mode
{
    NEAREST = 0
};

enum class pixel_format
{
    RED = 1,
    RG = 2,
    RGB = 3,
    RGBA = 4
};

class texture
{
    public:
        texture(int width, int height, wrap_mode wrap, filter_mode filter, pixel_format format, const unsigned char* data);
        ~texture();

        void bind();

        int get_width() const { return _width; };
        int get_height() const { return _height; };
    private:
        unsigned int _tbo;
        int _width, _height;
};

#endif //BESKAR_ENGINE_TEXTURE_H