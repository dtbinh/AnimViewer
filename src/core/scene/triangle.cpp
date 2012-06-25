/**
 * @file triangle.cpp
 * @brief Function definitions for the Triangle class.
 *
 * @author Eric Butler (edbutler)
 */

#include "lucPCH.h"

#include "scene/triangle.hpp"
#include "application/opengl.hpp"
#include "math/ray.hpp"
#include "app/raycasting.hpp"

namespace Luc {

Triangle::Triangle()
{
    vertices[0].material = 0;
    vertices[1].material = 0;
    vertices[2].material = 0;
}

Triangle::~Triangle() { }

void Triangle::render() const
{
    bool materials_nonnull = true;
    for ( int i = 0; i < 3; ++i )
        materials_nonnull = materials_nonnull && vertices[i].material;

    // this doesn't interpolate materials. Ah well.
    if ( materials_nonnull )
        vertices[0].material->set_gl_state();

    glBegin(GL_TRIANGLES);

    glNormal3fv( &vertices[0].normal.x );
    glTexCoord2fv( &vertices[0].tex_coord.x );
    glVertex3fv( &vertices[0].position.x );

    glNormal3fv( &vertices[1].normal.x );
    glTexCoord2fv( &vertices[1].tex_coord.x );
    glVertex3fv( &vertices[1].position.x);

    glNormal3fv( &vertices[2].normal.x );
    glTexCoord2fv( &vertices[2].tex_coord.x );
    glVertex3fv( &vertices[2].position.x);

    glEnd();

    if ( materials_nonnull )
        vertices[0].material->reset_gl_state();
}

// ray casting algorithm, check if a given line will hit this geometry.
// line is already in geomtry's coordinates
bool Triangle::ray_casting(const Ray&      line,
                           const real_t    tMin, 
                           const real_t    tMax,
                           float&          t, 
                           HitVertexInfor& hit_vertex) 
{
    Vertex v0 = vertices[0];
    Vertex v1 = vertices[1];
    Vertex v2 = vertices[2];

    // transform ray to geometry's coordinates
    Ray local_ray( (m_invTransformMat * Vector4(line.Point(), 1)).xyz(),
                   (m_invTransformMatWithoutTranslation * Vector4(line.Direction(), 1)).xyz() );

    float beta, gamma;
    if (!ray_casting_triangle(local_ray, v0.position, v1.position, v2.position, 
                              tMin, tMax, t, beta, gamma))
        return false;

    // compute mesh_vertex normal and materials
    hit_vertex.ambient = barycentric_interpolation(v0.material->ambient, v1.material->ambient, v2.material->ambient, beta, gamma);
    hit_vertex.diffuse = barycentric_interpolation(v0.material->diffuse, v1.material->diffuse, v2.material->diffuse, beta, gamma);
    hit_vertex.normal  = barycentric_interpolation(v0.normal, v1.normal, v2.normal, beta, gamma);
    hit_vertex.position = barycentric_interpolation(v0.position, v1.position, v2.position, beta, gamma);
    hit_vertex.refractive_index = barycentric_interpolation(v0.material->refractive_index, v1.material->refractive_index, v2.material->refractive_index, beta, gamma);
    hit_vertex.specular = barycentric_interpolation(v0.material->specular, v1.material->specular, v2.material->specular, beta, gamma);

    // transform to global coordinates
    hit_vertex.position = (m_transformatMat * Vector4(hit_vertex.position, 1)).xyz();
    hit_vertex.normal   = normalize(m_normalMatrix * hit_vertex.normal);

    // texture color
    Vector2 tex_coord;
    tex_coord.x = barycentric_interpolation(v0.tex_coord.x, v1.tex_coord.x, v2.tex_coord.x, beta, gamma);
    tex_coord.y = barycentric_interpolation(v0.tex_coord.y, v1.tex_coord.y, v2.tex_coord.y, beta, gamma);

    // blending three textures
    Color3 v0_tex_color = interpolate_texture_color(tex_coord, v0.material);
    Color3 v1_tex_color = interpolate_texture_color(tex_coord, v1.material);
    Color3 v2_tex_color = interpolate_texture_color(tex_coord, v2.material);

    hit_vertex.tex_color = barycentric_interpolation(v0_tex_color, v1_tex_color, v2_tex_color, beta, gamma);

    return true;
}


} /* Luc */

