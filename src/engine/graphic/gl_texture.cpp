#include "beskar_engine/gl_texture.h"

#include "GL/glew.h"

gl_texture::gl_texture(int width, int height, int channels, const unsigned char* data) : _width(width), _height(height)
{
    glGenTextures(1, &_tbo);
    glBindTexture(GL_TEXTURE_2D, _tbo);

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned int format = 0;
    switch(channels)
    {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

gl_texture::~gl_texture()
{
    glDeleteTextures(1, &_tbo);
}

void gl_texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _tbo);
}