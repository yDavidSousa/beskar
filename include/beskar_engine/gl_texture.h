#ifndef BESKAR_GL_TEXTURE_H
#define BESKAR_GL_TEXTURE_H

#include <iostream>

class gl_texture
{
    public:
        gl_texture(int width, int height, int channels, const unsigned char* data);
        ~gl_texture();

        void bind();

        int get_width() const { return _width; };
        int get_height() const { return _height; };
    private:
        unsigned int _tbo;
        int _width, _height;
};

#endif //BESKAR_GL_TEXTURE_H