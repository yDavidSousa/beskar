#include "beskar_engine/gl_mesh.h"

#include "GL/glew.h"

gl_mesh::gl_mesh(const float* vertices, unsigned int vert_length, const unsigned int* indices, unsigned int ind_length, const float* tex_coords, unsigned int tex_coords_length)
    : vertices_length(vert_length), indices_length(ind_length), tex_coords_length(tex_coords_length)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    if(vert_length)
    {
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, vert_length * sizeof(float), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (tex_coords_length)
    {
        glGenBuffers(1, &vbo_tex_coords);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coords);
        glBufferData(GL_ARRAY_BUFFER, tex_coords_length * sizeof(float), tex_coords, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if(ind_length)
    {
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_length * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(0);
}

gl_mesh::~gl_mesh()
{
    if (ibo) glDeleteBuffers(1, &ibo);
    if (vbo_vertices) glDeleteBuffers(1, &vbo_vertices);
    if (vbo_tex_coords) glDeleteBuffers(1, &vbo_tex_coords);
    glDeleteVertexArrays(1, &vao);
}

void gl_mesh::draw()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, indices_length, GL_UNSIGNED_INT, 0);
}