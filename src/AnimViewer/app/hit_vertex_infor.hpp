#pragma once
#ifndef HIT_VERTEX_INFORMATION_H
#define HIT_VERTEX_INFORMATION_H

#include "math/vector.hpp"
#include "math/color.hpp"

namespace Luc{

/*
 * Define a structure to record all necessary intersection point information 
 * for ray tracing algorithm.
 */
struct HitVertexInfor
{
    Vector3 position;
    Vector3 normal;
    
    // texture color
    Color3 tex_color;
    
    // ambient color (ignored if refractive_index != 0)
    Color3 ambient;
    // diffuse color
    Color3 diffuse;
    // specular (reflective) color
    Color3 specular;

    // refractive index of material dielectric. 0 is special case for
    // infinity, i.e. opaque. Any other value means transparent with the
    // given refractive index.
    real_t refractive_index;
};

}

#endif // HIT_VERTEX_INFORMATION_H

