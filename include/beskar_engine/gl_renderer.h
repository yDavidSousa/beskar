#ifndef BESKAR_GL_RENDERER_H
#define BESKAR_GL_RENDERER_H

#include "gl_shader.h"
#include "gl_mesh.h"
#include "gl_texture.h"
#include "sprite_asset.h"

struct vertex_data
{
    float x, y;
    float u, v;
};

struct batch_command
{
    unsigned int vao;
    unsigned int vbo;
    vertex_data* vertices;
    int vertex_count, vertex_capacity;

    unsigned int ibo;
    unsigned int* indices;
    int indices_count;

    batch_command(int capacity);
};

class gl_renderer
{
    public:
        gl_renderer();
        ~gl_renderer();

        [[nodiscard]] std::unique_ptr<gl_shader> create_shader(const char* vert_source, const char* frag_source);
        [[nodiscard]] std::unique_ptr<gl_mesh> create_mesh(const float* vertices, unsigned long vert_length, const unsigned int* indices, unsigned long ind_length, const float* tex_coords, unsigned long tex_coords_length);
        [[nodiscard]] std::unique_ptr<gl_texture> create_texture(const char* path);

        void push_quad(glm::vec2 position, glm::vec2 scale, const float* tex_coords);
        void draw();
    private:
        batch_command* batch_cmd;
};

#endif //BESKAR_GL_RENDERER_H