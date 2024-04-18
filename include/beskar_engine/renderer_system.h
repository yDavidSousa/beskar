#ifndef BESKAR_ENGINE_RENDERER_SYSTEM_H
#define BESKAR_ENGINE_RENDERER_SYSTEM_H

#include <glm/gtc/matrix_transform.hpp>

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

const unsigned int QUAD_VERTICES_LENGTH = 8;
const float QUAD_VERTICES[QUAD_VERTICES_LENGTH] =
{
        -0.5f, -0.5f, // bottom left
        0.5f, -0.5f, // bottom right
        0.5f,  0.5f, // top right
        -0.5f,  0.5f,  // top left
};

const unsigned int QUAD_INDICES_LENGTH = 6;
const unsigned int QUAD_INDICES[QUAD_INDICES_LENGTH] =
{
        0, 1, 2,  // first triangle
        0, 3, 2   // second triangle
};

const unsigned int QUAD_TEX_COORDS_LENGTH = QUAD_VERTICES_LENGTH;
const float QUAD_TEX_COORDS[QUAD_TEX_COORDS_LENGTH] =
{
        0.0f, 0.0f, // bottom left
        1.0f, 0.0f, // bottom right
        1.0f, 1.0f, // top right
        0.0f, 1.0f  // top left
};

class renderer_system
{
    public:
        renderer_system();
        ~renderer_system();

        void push_quad(glm::vec2 position, glm::vec2 scale, const float* tex_coords);
        void draw();
    private:
        batch_command* batch_cmd;
};

#endif //BESKAR_ENGINE_RENDERER_SYSTEM_H