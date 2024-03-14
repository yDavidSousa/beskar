#define STB_IMAGE_IMPLEMENTATION

#include "beskar_engine/gl_renderer.h"

#include "stb_image.h"
#include "GL/glew.h"

batch_command::batch_command(int capacity) : vertex_capacity(capacity), vertex_count(0), indices_count(0)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_capacity * sizeof(vertex_data), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*) offsetof(vertex_data, x));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*) offsetof(vertex_data, u));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (vertex_capacity * 2 * sizeof(unsigned int)), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    vertices = static_cast<vertex_data *>(malloc(vertex_capacity * sizeof(vertex_data)));
    indices = static_cast<unsigned int *>(malloc(vertex_capacity * 2 * sizeof(unsigned int)));
}

gl_renderer::gl_renderer()
{
    if(glewInit() != GLEW_OK)
    {
        return;
    }

    glEnable(GL_TEXTURE_2D);

    batch_cmd = new batch_command(1000);
}

gl_renderer::~gl_renderer()
{
    free(batch_cmd->vertices);
    free(batch_cmd->indices);
    delete batch_cmd;
}

std::unique_ptr<gl_shader> gl_renderer::create_shader(const char* vert_source, const char* frag_source)
{
    auto shader_instance = std::make_unique<gl_shader>(vert_source, frag_source);
    return shader_instance;
}

std::unique_ptr<gl_mesh> gl_renderer::create_mesh(const float* vertices, unsigned long vert_length, const unsigned int* indices, unsigned long ind_length, const float* tex_coords, unsigned long tex_coords_length)
{
    auto mesh_instance = std::make_unique<gl_mesh>(vertices, vert_length, indices, ind_length, tex_coords, tex_coords_length);
    return mesh_instance;
}

std::unique_ptr<gl_texture> gl_renderer::create_texture(const char* path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if(!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    auto texture_instance = std::make_unique<gl_texture>(width, height, channels, data);
    stbi_image_free(data);
    return texture_instance;
}

void gl_renderer::push_quad(glm::vec2 position, glm::vec2 scale, const float* tex_coords)
{
    if(batch_cmd->vertex_count == batch_cmd->vertex_capacity)
    {
        draw();
    }

    for (int i = 0; i < QUAD_INDICES_LENGTH; ++i)
    {
        batch_cmd->indices[batch_cmd->indices_count++] = QUAD_INDICES[i] + batch_cmd->vertex_count;
    }

    for (int i = 0; i < QUAD_VERTICES_LENGTH; i += 2)
    {
        vertex_data data;
        data.x = position.x + (QUAD_VERTICES[i] * scale.x);
        data.y = position.y + (QUAD_VERTICES[i+1] * scale.y);
        data.u = tex_coords[i];
        data.v = tex_coords[i+1];

        batch_cmd->vertices[batch_cmd->vertex_count++] = data;
    }
}

void gl_renderer::draw()
{
    if(batch_cmd->vertex_count == 0) return;

    glBindVertexArray(batch_cmd->vao);

    glBindBuffer(GL_ARRAY_BUFFER, batch_cmd->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch_cmd->vertex_count * sizeof(vertex_data), batch_cmd->vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch_cmd->ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batch_cmd->indices_count * sizeof(unsigned int), batch_cmd->indices);

    glDrawElements(GL_TRIANGLES, batch_cmd->indices_count, GL_UNSIGNED_INT, 0);

    batch_cmd->vertex_count = 0;
    batch_cmd->indices_count = 0;
}