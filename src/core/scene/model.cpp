/**
 * @file model.cpp
 * @brief Model class
 *
 * @author Eric Butler (edbutler)
 * @author Zeyang Li (zeyangl)
 */
#include "lucPCH.h"
#include "app/raycasting.hpp"
#include "scene/model.hpp"
#include "scene/material.hpp"
#include "math/ray.hpp"

#include <GL/gl.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>


namespace Luc {

Model::Model() : mesh( 0 ), material( 0 ) { }
Model::~Model() { }

void Model::render() const
{
    if ( !mesh )
        return;
    if ( material )
        material->set_gl_state();
    mesh->render();
    if ( material )
        material->reset_gl_state();
}

// ray casting algorithm, check if a given line will hit this geometry.
// line is already in geomtry's coordinates
bool Model::ray_casting(const Ray&      line,
                        const real_t    tMin, 
                        const real_t    tMax,
                        float&          t, 
                        HitVertexInfor& hit_vertex) 
{
    if (m_bounding_box.IsEmpty())
        build_bounding_box();

    if (!m_bounding_box.ray_casting(line, tMin, tMax, t))
        return false;

    // transform ray to geometry's coordinates
    Vector4 linePosV4(line.Point(), 1);
    Vector4 lineDirV4(line.Direction(), 1);
    Ray local_ray( (m_invTransformMat * linePosV4).xyz(),
                   (m_invTransformMatWithoutTranslation * lineDirV4).xyz() );

    const MeshTriangle* triangles = mesh->get_triangles();
    const MeshVertex*   vertices  = mesh->get_vertices();
    float closest_t = tMax+1;
    size_t closest_triangle_index = 0;
    float closest_triangle_beta  = 0;
    float closest_triangle_gamma = 0;

    // find closest hit point
    for (size_t i=0; i<mesh->num_triangles(); i++)
    {
        Vector3 p0 = vertices[triangles[i].vertices[0]].position;
        Vector3 p1 = vertices[triangles[i].vertices[1]].position;
        Vector3 p2 = vertices[triangles[i].vertices[2]].position;
        float current_t = 0;
        float beta, gamma;
        if (ray_casting_triangle(local_ray, p0, p1, p2, tMin, tMax, current_t, beta, gamma))
            if (current_t > tMin && current_t < tMax)
            {
                if (closest_t>current_t)
                {
                    closest_t =  current_t;
                    closest_triangle_beta  = beta;
                    closest_triangle_gamma = gamma;
                    closest_triangle_index = i;
                }
            }
    }
    if (closest_t > tMax)
        return false;

    t = closest_t;

    // compute mesh_vertex normal and materials
    hit_vertex.ambient = material->ambient;
    hit_vertex.diffuse = material->diffuse;
    hit_vertex.refractive_index = material->refractive_index;
    hit_vertex.specular = material->specular;

    // normal and position
    MeshVertex v0 = vertices[triangles[closest_triangle_index].vertices[0]];
    MeshVertex v1 = vertices[triangles[closest_triangle_index].vertices[1]];
    MeshVertex v2 = vertices[triangles[closest_triangle_index].vertices[2]];

    hit_vertex.normal  = barycentric_interpolation(v0.normal, v1.normal, v2.normal, closest_triangle_beta, closest_triangle_gamma);
    hit_vertex.position = barycentric_interpolation(v0.position, v1.position, v2.position, closest_triangle_beta, closest_triangle_gamma);

    // transform to global coordinates
    hit_vertex.position = (m_transformatMat * Vector4(hit_vertex.position, 1)).xyz();
    hit_vertex.normal   = normalize(m_normalMatrix * hit_vertex.normal);

    // texture color
    Vector2 tex_coord;
    tex_coord.x = barycentric_interpolation(v0.tex_coord.x, v1.tex_coord.x, v2.tex_coord.x, closest_triangle_beta, closest_triangle_gamma);
    tex_coord.y = barycentric_interpolation(v0.tex_coord.y, v1.tex_coord.y, v2.tex_coord.y, closest_triangle_beta, closest_triangle_gamma);

    hit_vertex.tex_color = interpolate_texture_color(tex_coord, material);

    return true;
}

void Model::build_bounding_box()
{
    std::cout << "Start build bounding box..." << std::endl;
    size_t num_vert = mesh->num_vertices();
    const MeshVertex* mVeretx = mesh->get_vertices();
    for (size_t i=0; i<num_vert; ++i)
    {
        Vector3 vertex = mVeretx[i].position;
        vertex = (m_transformatMat * Vector4(vertex, 1)).xyz();
        m_bounding_box.filter_vertex(vertex);
    }
    std::cout << "finished building bounding box" << std::endl;
}

} /* Luc */

