#ifndef BESKAR_ENGINE_SHADER_H
#define BESKAR_ENGINE_SHADER_H

#include "glm/gtc/matrix_transform.hpp"

class shader
{
    public:
        shader(const char* vert_source, const char* frag_source);
        ~shader();

        void use();
        void set_uniform_mat4(const char *name, glm::mat4 value);
        void set_uniform_4fv(const char *name, float value[4]);
    private:
        unsigned int m_program;
};

#endif //BESKAR_ENGINE_SHADER_H