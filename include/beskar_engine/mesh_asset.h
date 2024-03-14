#ifndef BESKAR_MESH_ASSET_H
#define BESKAR_MESH_ASSET_H

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

struct mesh_asset
{
    float* vertices;
    unsigned long vertices_length;
    unsigned int* indices;
    unsigned long indices_length;

    mesh_asset();
    mesh_asset(float* vertices, unsigned long vertices_length, unsigned int* indices, unsigned long indices_length);

    static const mesh_asset get_primitive_quad();
};

#endif //BESKAR_MESH_ASSET_H