#include "beskar_engine/texture.h"

#include "GL/glew.h"

texture::texture(int width, int height, wrap_mode wrap, filter_mode filter, pixel_format format, const unsigned char* data) : _width(width), _height(height)
{
    glGenTextures(1, &_tbo);
    glBindTexture(GL_TEXTURE_2D, _tbo);

    // Wrapping
    GLint gl_wrap;
    switch (wrap)
    {
        case wrap_mode::CLAMP:
            gl_wrap = GL_CLAMP;
            break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap);

    // Filtering
    GLint gl_filter;
    switch (filter)
    {
        case filter_mode::NEAREST:
            gl_filter = GL_NEAREST;
            break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter);

    GLenum gl_format = 0;
    switch (format)
    {
        case pixel_format::RED:
            gl_format = GL_RED;
            break;
        case pixel_format::RG:
            gl_format = GL_RG;
            break;
        case pixel_format::RGB:
            gl_format = GL_RGB;
            break;
        case pixel_format::RGBA:
            gl_format = GL_RGBA;
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

texture::~texture()
{
    glDeleteTextures(1, &_tbo);
}

void texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _tbo);
}