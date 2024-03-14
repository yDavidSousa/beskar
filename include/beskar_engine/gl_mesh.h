#ifndef BESKAR_GL_MESH_H
#define BESKAR_GL_MESH_H

#include "mesh_asset.h"

class gl_mesh
{
    public:
        gl_mesh(const float* vertices, unsigned int vert_length, const unsigned int* indices, unsigned int ind_length, const float* tex_coords, unsigned int tex_coords_length);
        ~gl_mesh();

        void draw();
    private:
        unsigned int vao;
        unsigned int vbo_vertices, vertices_length;
        unsigned int vbo_tex_coords, tex_coords_length;
        unsigned int ibo, indices_length;
};

#endif //BESKAR_GL_MESH_H