#include "beskar_engine/gl_shader.h"

#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "GL/glew.h"

const char* STANDARD_VERT_SOURCE = "#version 330 core\n"
    "layout (location = 0) in vec2 a_vert;\n"
    "layout (location = 1) in vec2 a_tex;\n"
    "out vec2 v_tex;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "uniform mat4 u_model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = u_projection * u_view * u_model * vec4(a_vert.xy, 0.0f, 1.0f);\n"
    "   v_tex = a_tex;\n"
    "}\0";
const char* STANDARD_FRAG_SOURCE = "#version 330 core\n"
    "out vec4 o_color;\n"
    "in vec2 v_tex;\n"
    "uniform sampler2D u_texture;\n"
    "void main()\n"
    "{\n"
    "   o_color = texture(u_texture, v_tex);\n"
    //"   o_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\0";

gl_shader::gl_shader(const char* vert_source, const char* frag_source)
{
    m_program = glCreateProgram();

    int success;
    char infoLog[512];

    //Compile Vertex Shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Compile Fragment Shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Link
    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //FIND UNIFORMS HERE

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

gl_shader::~gl_shader()
{
    glDeleteProgram(m_program);
}

void gl_shader::use()
{
    glUseProgram(m_program);
}

void gl_shader::set_uniform_mat4(const char *name, glm::mat4 value)
{
    unsigned int location = glGetUniformLocation(m_program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}