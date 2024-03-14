#include "beskar_engine/mesh_asset.h"

mesh_asset::mesh_asset()
{
}

mesh_asset::mesh_asset(float* vertices, unsigned long vertices_length, unsigned int* indices, unsigned long indices_length) 
: vertices(vertices), vertices_length(vertices_length), indices(indices), indices_length(indices_length)
{
}

const mesh_asset mesh_asset::get_primitive_quad()
{
    mesh_asset quad_mesh;
    quad_mesh.vertices = const_cast<float*>(QUAD_VERTICES);
    quad_mesh.vertices_length = QUAD_VERTICES_LENGTH;
    quad_mesh.indices = const_cast<unsigned int*>(QUAD_INDICES);
    quad_mesh.indices_length = QUAD_INDICES_LENGTH;
    return quad_mesh;
}