#ifndef BESKAR_GL_SHADER_H
#define BESKAR_GL_SHADER_H

#include "glm/gtc/matrix_transform.hpp"

extern const char* STANDARD_VERT_SOURCE;
extern const char* STANDARD_FRAG_SOURCE;

class gl_shader
{
    public:
        gl_shader(const char* vert_source, const char* frag_source);
        ~gl_shader();

        void use();
        void set_uniform_mat4(const char *name, glm::mat4 value);
    private:
        unsigned int m_program;
};

#endif //BESKAR_GL_SHADER_H